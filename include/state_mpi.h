#ifndef BASE_STATE_MPI_H
#define BASE_STATE_MPI_H
#include "state.h"

void initialize_mpi(int &argc, char **&argv, int &rank, int &size);

std::chrono::time_point<std::chrono::system_clock> start_timer(int rank);

int calculate_rows(int rank, int size, int height);

void load_rows(StateMatrix &state, const std::vector<int> &flat_data, int flat_size);

std::vector<int> flatten_data(StateMatrix &src);

StateMatrix process_part(int rank, int width, int regular_segment, int num_of_rows, const StateMatrix& state);

void event_loop_mpi(int height, int width, int num_of_iter);

#endif //BASE_STATE_MPI_H
