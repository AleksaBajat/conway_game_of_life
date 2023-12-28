#include <iostream>
#include <array>
#include <fstream>
#include <filesystem>
#include <thread>
#include <omp.h>
#define MAP_SIZE 10
#define PRINT

constexpr const char *state_file = "state.txt";
constexpr int map_size = MAP_SIZE;
using StateMatrix = std::array<std::array<bool, map_size>, map_size>;

StateMatrix init() {
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
#ifdef PRINT
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
#endif
}

int top_neighbour(const int i, const int j, const StateMatrix &previous_state) {
    if (i == 0) {
        return 0;
    }
    const int result = previous_state[i - 1][j] ? 1 : 0;
    return result;
}

int bottom_neighbour(const int i, const int j, const StateMatrix &previous_state) {
    if (i == map_size - 1) {
        return 0;
    }
    const int result = previous_state[i + 1][j] ? 1 : 0;
    return result;
}

int left_neighbour(const int i, const int j, const StateMatrix &previous_state) {
    if (j == 0) {
        return 0;
    }
    const int result = previous_state[i][j - 1] ? 1 : 0;
    return result;
}

int right_neighbour(const int i, const int j, const StateMatrix &previous_state) {
    if (j == map_size - 1) {
        return 0;
    }
    const int result = previous_state[i][j + 1] ? 1 : 0;
    return result;
}

int top_right_neighbour(const int i, const int j, const StateMatrix &previous_state){
    if(i == 0 || j == map_size - 1){
        return 0;
    }

    const int result = previous_state[i - 1][j + 1] ? 1 : 0;
    return result;
}

int top_left_neighbour(const int i, const int j, const StateMatrix &previous_state){
    if(i == 0 || j == 0){
        return 0;
    }

    const int result = previous_state[i - 1][j - 1] ? 1 : 0;
    return result;
}

int bottom_right_neighbour(const int i, const int j, const StateMatrix &previous_state){
    if(i == map_size - 1 || j == map_size - 1){
        return 0;
    }

    const int result = previous_state[i + 1][j + 1] ? 1 : 0;
    return result;
}

int bottom_left_neighbour(const int i, const int j, const StateMatrix &previous_state){
    if(i == map_size - 1 || j == 0){
        return 0;
    }

    const int result = previous_state[i + 1][j - 1] ? 1 : 0;
    return result;
}

bool next_state(const int i, const int j, const StateMatrix &previous_state) {
    const int number_of_neighbours = top_neighbour(i, j, previous_state)
                                     + bottom_neighbour(i, j, previous_state)
                                     + left_neighbour(i, j, previous_state)
                                     + right_neighbour(i, j, previous_state)
                                     + top_right_neighbour(i, j, previous_state)
                                     + top_left_neighbour(i, j, previous_state)
                                     + bottom_left_neighbour(i, j, previous_state)
                                     + bottom_right_neighbour(i, j, previous_state);

    if (previous_state[i][j]) {
        return number_of_neighbours == 2 || number_of_neighbours == 3;
    }

    return number_of_neighbours == 3;
}

[[noreturn]] void event_loop() {
    StateMatrix previous_state = init();
    print_state(previous_state);
    while (true) {
        StateMatrix new_state{};
        #pragma omp parallel for schedule(dynamic)
        for (int i = 0; i < map_size; ++i) {
            for (int j = 0; j < map_size; ++j) {
                new_state[i][j] = next_state(i, j, previous_state);
            }
        }
        print_state(new_state);
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        previous_state = new_state;
    }
}

int main(int argc, char **argv) {
    event_loop();
    return 0;
}
