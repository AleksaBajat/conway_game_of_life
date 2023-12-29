#include "include/state.h"

[[noreturn]] void event_loop() {
    StateMatrix previous_state = init_state();
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

int main() {
    std::cout << "HELLO WORLD" << std::endl;
    event_loop();
    return 0;
}
