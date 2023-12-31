#include <numeric>
#include "mpi.h"
#include "../include/state_mpi.h"

int calculate_rows(int rank, int size, int height) {
    if(rank != size - 1){
        return height / size;
    }else{
        return height / size + height % size;
    }
}

void event_loop_mpi(int height, int width, int num_of_iter) {
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    StateMatrix state = init_state(height, width);

    for(int i=0; i < num_of_iter ; i++){
        StateMatrix state_part = process_part(rank, size, height, width, state);
        std::vector<int> flattened = flatten_data(state_part);
        int flattened_size = static_cast<int>(flattened.size());

        std::vector<int> data_sizes(size);
        std::vector<int> displacements(size);

        MPI_Gather(&flattened_size, 1, MPI_INT, data_sizes.data(), 1, MPI_INT, 0, MPI_COMM_WORLD);

        if(rank == 0){
            int total_size = std::accumulate(data_sizes.begin(), data_sizes.end(), 0);
            std::vector<int> recvbuf(total_size);
            int offset = 0;

            for(int j=0; j< size; ++j){
                displacements[j] = offset;
                offset += data_sizes[j];
            }

            MPI_Gatherv(flattened.data(), flattened_size, MPI_INT,
                        recvbuf.data(), data_sizes.data(), displacements.data(), MPI_INT,
                        0, MPI_COMM_WORLD);

            state = {};
            load_rows(state, recvbuf, static_cast<int>(recvbuf.size()));
        }else{
            MPI_Gatherv(flattened.data(), flattened_size, MPI_INT,
                        nullptr, nullptr, nullptr, MPI_INT,
                        0, MPI_COMM_WORLD);
        }

#if !BENCH
        if(rank == 0){
            print_state(state, i);
        }
#endif

        std::vector<int> flat_data;
        if(rank == 0){
            flat_data = flatten_data(state);
        }

        int fs = static_cast<int>(flat_data.size());

        MPI_Bcast(&fs, 1, MPI_INT, 0, MPI_COMM_WORLD);

        if(rank != 0){
            flat_data.resize(fs);
        }

        MPI_Bcast(flat_data.data(), fs, MPI_INT, 0, MPI_COMM_WORLD);

        if(rank != 0){
            state = {};
            load_rows(state, flat_data, fs);
        }
    }
}

std::vector<int> flatten_data(StateMatrix &src) {
    std::vector<int> dst{};
    for(auto &inner : src){
        dst.push_back(static_cast<int>(inner.size()));
        for (bool val: inner){
            dst.push_back(static_cast<int>(val));
        }
    }

    return dst;
}

void load_rows(StateMatrix &state, const std::vector<int> &flat_data, int flat_size) {
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


StateMatrix process_part(int rank, int size, int height, int width, const StateMatrix &state) {
    int num_of_rows = calculate_rows(rank, size, height);
    int regular_segment = height/size;
    StateMatrix state_part{};
    for(int i=rank*regular_segment; i < rank * regular_segment + num_of_rows; i++){
        std::vector<bool> inner{};
        for(int j=0; j < width; j++){
            inner.push_back(next_node_state(i, j, state));
        }
        state_part.push_back(inner);
    }

    return state_part;
}
