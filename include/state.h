#ifndef BASE_STATE_H
#define BASE_STATE_H
#define MAP_SIZE 10

#include <iostream>
#include <array>
#include <fstream>
#include <filesystem>
#include <thread>
#include <omp.h>

constexpr const char *state_file = "state.txt";
constexpr int map_size = MAP_SIZE;
using StateMatrix = std::array<std::array<bool, map_size>, map_size>;

StateMatrix init_state();

void print_state(const StateMatrix &matrix, int iter_count = -1);

int get_neighbour(int i, int j, const StateMatrix &state, int di, int dj);

int get_number_of_neighbours(int i, int j, const StateMatrix &state);

bool next_node_state(int i, int j, const StateMatrix &state);

StateMatrix next_state(const StateMatrix &state);
#endif
