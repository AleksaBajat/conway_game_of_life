#define CATCH_CONFIG_RUNNER
#include <mpi.h>
#include <catch2/catch_test_macros.hpp>
#include <catch2/benchmark/catch_benchmark.hpp>
#include <catch2/catch_session.hpp>
#include "../include/state_mpi.h"

int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    Catch::Session session;

    std::streambuf* originalCout = std::cout.rdbuf();

    if (world_rank != 0) {
        std::ofstream devNull("/dev/null");
        std::cout.rdbuf(devNull.rdbuf());
    }

    int result = session.run(argc, argv);

    std::cout.rdbuf(originalCout);

    MPI_Finalize();
    return result;
}

TEST_CASE("next state - 100 iter, 100x100", "[100-100-100]"){
    BENCHMARK("next_state"){
                               return event_loop_mpi(100, 100, 100);
                           };
}

TEST_CASE("next state - 1000 iter, 100x100", "[1000-100-100]"){
    BENCHMARK("next_state"){
                               return event_loop_mpi(100, 100, 1000);
                           };
}

TEST_CASE("next state - 100 iter, 1000x1000", "[100-1000-1000]") {
    BENCHMARK("next_state") {
                                return event_loop_mpi(1000, 1000, 100);
                            };
}
