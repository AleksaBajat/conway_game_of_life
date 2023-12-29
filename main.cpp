#include "include/state.h"
#define ITER_COUNT 100

void event_loop() {
    StateMatrix state = init_state();
    int iter_count = 0;
    print_state(state, iter_count);
    while (iter_count++ != ITER_COUNT) {
        state = next_state(state);
        print_state(state, iter_count);
    }
}

int main() {
    event_loop();
    return 0;
}
