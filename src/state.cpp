#include "../include/state.h"

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

void print_state(const StateMatrix &matrix) {
    const std::string RED = "\033[31m";
    const std::string GREEN = "\033[32m";
    const std::string RESET = "\033[0m";
    for (int i = 0; i < map_size; i++) {
        for (int j = 0; j < map_size; j++) {
            // Print X in red and O in green
            std::cout << (matrix[i][j] ? GREEN : RED) << (matrix[i][j] ? "O " : "X ") << RESET;
        }

        std::cout << "\n";
    }
    std::cout << RED << "##################################" << RESET << std::endl;
}

int get_neighbour(const int i, const int j, const StateMatrix &previous_state, const int di, const int dj) {
    int new_i = i + di;
    int new_j = j + dj;

    if(new_i > map_size - 1 || new_i < 0 || new_j > map_size - 1 || new_j < 0){
        return 0;
    }

    const int result = previous_state[new_i][new_j] ? 1 : 0;
    return result;
}

int get_number_of_neighbours(const int i, const int j, const StateMatrix &previous_state) {
    int distances[8][2] = {{1, 0}, {1, 1}, {0, 1}, {-1, 1}, {-1, 0}, {-1, -1}, {0, -1}, {1, -1}};
    int number_of_neighbours = 0;
    for (auto & distance : distances)
        number_of_neighbours += get_neighbour(i, j, previous_state, distance[0], distance[1]);

    return number_of_neighbours;
}

bool next_state(const int i, const int j, const StateMatrix &previous_state) {
    int number_of_neighbours = get_number_of_neighbours(i, j, previous_state);

    if (previous_state[i][j]) {
        return number_of_neighbours == 2 || number_of_neighbours == 3;
    }

    return number_of_neighbours == 3;
}
