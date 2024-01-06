#include <cstring>
#include "include/state_mpi.h"

#ifndef ITER_COUNT
#define ITER_COUNT 3
#endif
#define HEIGHT 10
#define WIDTH 10
#define OPENMP_THREADS 4


int main(int argc, char** argv) {

    int mpiFlag = 0;
    int openmpFlag = 0;
    int openmptFlag = 0;

    for(int i = 1; i < argc; i++) {
        if(strcmp(argv[i], "--mpi") == 0) {
            mpiFlag = 1;
        } else if(strcmp(argv[i], "--openmp") == 0) {
            openmpFlag = 1;
        } else if(strcmp(argv[i], "--openmpt") == 0) {
            openmptFlag = 1;
        }
    }

    if(mpiFlag + openmpFlag + openmptFlag > 1) {
        printf("Multiple flags are present. Use --mpi, --openmp, --openmpt or nothing.\n");
    } else if(mpiFlag) {
        int rank;
        MPI_Comm_rank(MPI_COMM_WORLD, &rank);
        if(rank == 0){
            printf("--mpi is present.\n");
        }
        MPI_Init(&argc, &argv);
        event_loop_mpi(HEIGHT, WIDTH, ITER_COUNT);
        MPI_Finalize();
    } else if(openmpFlag) {
        omp_set_num_threads(OPENMP_THREADS);
        printf("--openmp is present.\n.\n");
        event_loop_omp(ITER_COUNT, HEIGHT, WIDTH);
    } else if(openmptFlag) {
        omp_set_num_threads(OPENMP_THREADS);
        printf("--openmpt is present.\n");
        event_loop_omp_task(ITER_COUNT, HEIGHT, WIDTH);
    }else {
        printf("Neither --mpi, --openmp nor --openmpt are present.\n");
        event_loop(ITER_COUNT, HEIGHT, WIDTH);
    }

    return 0;
}

