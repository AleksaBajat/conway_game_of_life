#include "../include/state.h"
#include <iostream>

StateMatrix init_state() {
    auto matrix = StateMatrix{};
    if (std::filesystem::exists(state_file)) {
        std::string line;
        std::ifstream state(state_file);
        int line_counter = 0;
        while (getline(state, line)) {
            int char_counter = 0;
            for (char c: line) {
                if (c == ' ') {
                    char_counter++;
                    continue;
                }
                matrix[line_counter][char_counter] = c == 'O';
            }
            line_counter++;
        }
        state.close();
    } else {
        std::ofstream state(state_file);
        for (int i = 0; i < map_size; i++) {
            for (int j = 0; j < map_size; j++) {
                state << "X ";
            }
            state << "\n";
        }
        state.close();
    }

    return matrix;
}

void print_state(const StateMatrix &matrix, int iter_count) {
    const std::string RED = "\033[31m";
    const std::string GREEN = "\033[32m";
    const std::string BLUE = "\033[34m";
    const std::string RESET = "\033[0m";
    std::cout << RED << "################" << BLUE << iter_count << RED << "#################" << RESET << std::endl;
    for (int i = 0; i < map_size; i++) {
        for (int j = 0; j < map_size; j++) {
            // Print X in red and O in green
            std::cout << (matrix[i][j] ? GREEN : RED) << (matrix[i][j] ? "O " : "X ") << RESET;
        }

        std::cout << "\n";
    }
}

int get_neighbour(const int i, const int j, const StateMatrix &state, const int di, const int dj) {
    int new_i = i + di;
    int new_j = j + dj;

    if(new_i > map_size - 1 || new_i < 0 || new_j > map_size - 1 || new_j < 0){
        return 0;
    }

    const int result = state[new_i][new_j] ? 1 : 0;
    return result;
}

int get_number_of_neighbours(const int i, const int j, const StateMatrix &state) {
    int distances[8][2] = {{1, 0}, {1, 1}, {0, 1}, {-1, 1}, {-1, 0}, {-1, -1}, {0, -1}, {1, -1}};
    int number_of_neighbours = 0;
    for (auto & distance : distances)
        number_of_neighbours += get_neighbour(i, j, state, distance[0], distance[1]);

    return number_of_neighbours;
}

bool next_node_state(const int i, const int j, const StateMatrix &state) {
    int number_of_neighbours = get_number_of_neighbours(i, j, state);

    if (state[i][j]) {
        return number_of_neighbours == 2 || number_of_neighbours == 3;
    }

    return number_of_neighbours == 3;
}

StateMatrix next_state(const StateMatrix &state) {
    StateMatrix new_state{};
    for (int i = 0; i < map_size; ++i) {
        for (int j = 0; j < map_size; ++j) {
            new_state[i][j] = next_node_state(i, j, state);
        }
    }
    return new_state;
}

void event_loop(int num_of_iters) {
    StateMatrix state = init_state();
    int iter_count = 0;
//    print_state(state, iter_count);
    while (iter_count++ != num_of_iters) {
        state = next_state(state);
//        print_state(state, iter_count);
    }
}


void event_loop_omp(int num_of_iters) {
    StateMatrix state = init_state();
    int iter_count = 0;
//    print_state(state, iter_count);
    while (iter_count++ != num_of_iters) {
        state = next_state_omp(state);
//        print_state(state, iter_count);
    }
}


StateMatrix next_state_omp(const StateMatrix &state) {
    StateMatrix new_state{};
#pragma omp parallel for
    for (int i = 0; i < map_size; ++i) {
        for (int j = 0; j < map_size; ++j) {
            new_state[i][j] = next_node_state(i, j, state);
        }
    }
    return new_state;
}

StateMatrix next_state_mpi(const StateMatrix &state) {
    StateMatrix new_state{};
    for (int i = 0; i < map_size; ++i) {
        for (int j = 0; j < map_size; ++j) {
            new_state[i][j] = next_node_state(i, j, state);
        }
    }
    return new_state;
}
