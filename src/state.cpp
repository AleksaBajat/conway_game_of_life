#include "../include/state.h"
#include <iostream>

StateMatrix init_state(const int height, const int width) {
    auto matrix = StateMatrix(height, std::vector<bool>(width, false));
    std::string state_file = "state" + std::to_string(height) + "x" + std::to_string(width) + ".txt";
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
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                state << "X ";
            }
            state << "\n";
        }
        state.close();
    }

    return matrix;
}

void print_state(const StateMatrix &state, int iter_count) {
    if(state.empty() || state[0].empty()){
        std::cout << "EMPTY" << std::endl;
        return;
    }
    const unsigned long height = state.size();
    const unsigned long width = state[0].size();
    const std::string RED = "\033[31m";
    const std::string GREEN = "\033[32m";
    const std::string BLUE = "\033[34m";
    const std::string RESET = "\033[0m";
    std::string bounds(width - 2, '#');
    std::cout << RED << bounds << BLUE << iter_count << RED << bounds << RESET << std::endl;
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            // Print X in red and O in green
            std::cout << (state[i][j] ? GREEN : RED) << (state[i][j] ? "O " : "X ") << RESET;
        }

        std::cout << "\n";
    }
}

int get_neighbour(const int i, const int j, const StateMatrix &state, const int di, const int dj) {
    const unsigned long height = state.size();
    const unsigned long width = state[0].size();
    int new_i = i + di;
    int new_j = j + dj;

    if(new_i > height - 1 || new_i < 0 || new_j > width - 1 || new_j < 0){
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
    const unsigned long height = state.size();
    const unsigned long width = state[0].size();
    auto new_state = StateMatrix(height, std::vector<bool>(width, false));
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            new_state[i][j] = next_node_state(i, j, state);
        }
    }
    return new_state;
}

void event_loop(int num_of_iters, const int height, const int width) {
    StateMatrix state = init_state(height, width);
    int iter_count = 0;
#if !BENCH
    print_state(state, iter_count);
#endif
    while (iter_count++ != num_of_iters) {
        state = next_state(state);
#if !BENCH
        print_state(state, iter_count);
#endif
    }
}


void event_loop_omp(int num_of_iters, int height, int width) {
    StateMatrix state = init_state(height, width);
    int iter_count = 0;
#if !BENCH
    print_state(state, iter_count);
#endif
    while (iter_count++ != num_of_iters) {
        state = next_state_omp(state);
#if !BENCH
        print_state(state, iter_count);
#endif
    }
}

StateMatrix next_state_omp(const StateMatrix &state) {
    const unsigned long height = state.size();
    const unsigned long width = state[0].size();
    auto new_state = StateMatrix(height, std::vector<bool>(width, false));
#pragma omp parallel for schedule(dynamic)
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            new_state[i][j] = next_node_state(i, j, state);
        }
    }
    return new_state;
}


void event_loop_omp_task(int num_of_iters, int height, int width) {
    StateMatrix state = init_state(height, width);
    int iter_count = 0;
#if !BENCH
    print_state(state, iter_count);
#endif
    while (iter_count++ != num_of_iters) {
        state = next_state_omp_task(state);
#if !BENCH
        print_state(state, iter_count);
#endif
    }
}


StateMatrix next_state_omp_task(const StateMatrix &state) {
    const unsigned long height = state.size();
    const unsigned long width = state[0].size();
    auto new_state = StateMatrix(height, std::vector<bool>(width, false));

#pragma omp parallel
    {
#pragma omp single
        {
            for (int i = 0; i < height; ++i) {
#pragma omp task firstprivate(i)
                {
                    for (int j = 0; j < width; ++j) {
                        new_state[i][j] = next_node_state(i, j, state);
                    }
                }
            }
        }
    }

    return new_state;
}


