#include "include/state_mpi.h"

#ifndef ITER_COUNT
#define ITER_COUNT 100
#endif
#define HEIGHT 100
#define WIDTH 100


int main(int argc, char** argv) {
    event_loop_mpi(argc, argv, HEIGHT, WIDTH, ITER_COUNT);

    return 0;
}

