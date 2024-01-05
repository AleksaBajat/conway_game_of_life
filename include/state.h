#ifndef BASE_STATE_H
#define BASE_STATE_H

#include <iostream>
#include <fstream>
#include <vector>
#include <filesystem>
#include <thread>
#include <omp.h>
#include "mpi.h"

using StateMatrix = std::vector<std::vector<bool>>;
StateMatrix init_state(int height, int width);

void print_state(const StateMatrix &state, int iter_count = 0);

int get_neighbour(int i, int j, const StateMatrix &state, int di, int dj);

int get_number_of_neighbours(int i, int j, const StateMatrix &state);

bool next_node_state(int i, int j, const StateMatrix &state);

StateMatrix next_state(const StateMatrix &state);
StateMatrix next_state_omp(const StateMatrix &state);

void event_loop(int num_of_iters, int height, int width);
void event_loop_omp(int num_of_iters, int height, int width);
#endif
