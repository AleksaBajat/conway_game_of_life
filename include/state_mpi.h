#ifndef BASE_STATE_MPI_H
#define BASE_STATE_MPI_H
#include "state.h"

int calculate_rows(int rank, int size, int height);

void load_rows(StateMatrix &state, const std::vector<int> &flat_data, int flat_size);

std::vector<int> flatten_data(StateMatrix &src);

StateMatrix process_part(int rank, int size, int height, int width, const StateMatrix& state);

void event_loop_mpi(int height, int width, int num_of_iter);

#endif //BASE_STATE_MPI_H
