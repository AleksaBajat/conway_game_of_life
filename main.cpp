#include "include/state.h"
#ifndef ITER_COUNT
#define ITER_COUNT 100
#endif

int main() {
    event_loop_omp(ITER_COUNT);
    return 0;
}

