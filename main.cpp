#include "include/state.h"
#include <chrono>
#include <vector>
#include <numeric>

#ifndef ITER_COUNT
#define ITER_COUNT 100
#endif
#define HEIGHT 100
#define WIDTH 100

void load_rows(int rank, StateMatrix &state, const std::vector<int> &flat_data, int flat_size);

void flatten_data(StateMatrix &src, std::vector<int> &dst, int &flat_size);

void process_part(int rank,int regular_segment, int num_of_rows, const StateMatrix& state, StateMatrix &state_part);

void event_loop_mpi(int &argc, char **&argv);

int main(int argc, char** argv) {
    auto time_start = std::chrono::system_clock::now();

    omp_set_num_threads(5);
    event_loop_omp(100, 100, 100);

    auto time_end = std::chrono::system_clock::now();

    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(time_end - time_start);
    std::cout << "Elapsed time: " << elapsed.count() << std::endl;

    return 0;
}

void event_loop_mpi(int &argc, char **&argv) {
    MPI_Init(&argc, &argv);


    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    std::chrono::time_point<std::chrono::system_clock> time_start;

    if(rank == 0){
        time_start = std::chrono::system_clock::now();
    }

    StateMatrix state;
    std::vector<int> flat_data;
    int flat_size;
    if(rank == 0){
        state = init_state(HEIGHT, WIDTH);
        flatten_data(state, flat_data, flat_size);
    }

    MPI_Bcast(&flat_size, 1, MPI_INT, 0, MPI_COMM_WORLD);

    if(rank != 0){
        flat_data.resize(flat_size);
    }

    MPI_Bcast(flat_data.data(), flat_size, MPI_INT, 0, MPI_COMM_WORLD);

    if(rank != 0){
        load_rows(rank, state, flat_data, flat_size);
    }

    StateMatrix state_part{};

    int rows_to_do;
    if(rank != size - 1){
        rows_to_do = HEIGHT / size;
    }else{
        rows_to_do = HEIGHT / size + HEIGHT % size;
    }

    for(int i=0; i < ITER_COUNT; i++){
        state_part = {};
        process_part(rank, HEIGHT/size, rows_to_do, state, state_part);
        std::vector<int> flattened{};
        int flattened_size;
        flatten_data(state_part, flattened, flattened_size);

        std::vector<int> recvcounts(size);  // size is the number of processes
        std::vector<int> displs(size);

        MPI_Gather(&flattened_size, 1, MPI_INT, recvcounts.data(), 1, MPI_INT, 0, MPI_COMM_WORLD);

        if(rank == 0){
            int total_size = std::accumulate(recvcounts.begin(), recvcounts.end(), 0);
            std::vector<int> recvbuf(total_size);
            int offset = 0;

            for(int j=0; j< size; ++j){
                displs[j] = offset;
                offset += recvcounts[j];
            }

          MPI_Gatherv(flattened.data(), flattened_size, MPI_INT,
            recvbuf.data(), recvcounts.data(), displs.data(), MPI_INT,
            0, MPI_COMM_WORLD);

            state = {};
            load_rows(rank, state, recvbuf, static_cast<int>(recvbuf.size()));
        }else{
            MPI_Gatherv(flattened.data(), flattened_size, MPI_INT,
                nullptr, nullptr, nullptr, MPI_INT,
                0, MPI_COMM_WORLD);
        }

        if(rank == 0){
//            print_state(state, i);
            std::cout << "ITER " << i << std::endl;
        }

        std::vector<int> fd;
        int fs;
        if(rank == 0){
            flatten_data(state, fd, fs);
        }

        MPI_Bcast(&fs, 1, MPI_INT, 0, MPI_COMM_WORLD);

        if(rank != 0){
            fd.resize(fs);
        }

        MPI_Bcast(fd.data(), fs, MPI_INT, 0, MPI_COMM_WORLD);

        if(rank != 0){
            state = {};
            load_rows(rank, state, fd, fs);
        }
    }

    if(rank == 0){
        auto time_end = std::chrono::system_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(time_end - time_start);
        std::cout << "Elapsed time: " << elapsed.count() << std::endl;
    }


    MPI_Finalize();
}

void flatten_data(StateMatrix &src, std::vector<int> &dst, int &flat_size) {
    for(auto &inner : src){
        dst.push_back(static_cast<int>(inner.size()));
        for (bool val: inner){
            dst.push_back(static_cast<int>(val));
        }
    }

    flat_size = static_cast<int>(dst.size());
}

void load_rows(int rank, StateMatrix &state, const std::vector<int> &flat_data, int flat_size) {
    int pos = 0;
    while(pos < flat_size){
        int inner_size = flat_data[pos++];
        std::vector<bool> inner_vec;
        for(int i=0; i< inner_size;i++){
            inner_vec.push_back(static_cast<bool>(flat_data[pos++]));
        }

        state.push_back(inner_vec);
    }

}

void process_part(int rank, int regular_segment, int num_of_rows, const StateMatrix& state, StateMatrix &state_part){
    for(int i=rank*regular_segment; i < rank * regular_segment + num_of_rows; i++){
        std::vector<bool> inner{};
        for(int j=0; j < WIDTH; j++){
            inner.push_back(next_node_state(i, j, state));
        }
        state_part.push_back(inner);
    }
}