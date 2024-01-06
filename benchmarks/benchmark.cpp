#include <catch2/catch_test_macros.hpp>
#include "catch2/benchmark/catch_benchmark.hpp"
#include "../include/state.h"


TEST_CASE("next state - 100 iter, 100x100", "[100-100-100]"){
    BENCHMARK("sequential 100i-100x100"){
                               return event_loop(100, 100, 100);
                           };
    BENCHMARK("omp 2 100i-100x100"){
                                     omp_set_num_threads(2);
                                     return event_loop_omp(100, 100, 100);
                                 };

    BENCHMARK("omp 4 100i-100x100"){
                                     omp_set_num_threads(4);
                                     return event_loop_omp(100, 100, 100);
                                 };

    BENCHMARK("omp 8 100i-100x100"){
                                     omp_set_num_threads(8);
                                     return event_loop_omp(100, 100, 100);
                                 };

    BENCHMARK("omp_task 2 100i-100x100"){
                                     omp_set_num_threads(2);
                                     return event_loop_omp_task(100, 100, 100);
                                 };

    BENCHMARK("omp_task 4 100i-100x100"){
                                     omp_set_num_threads(4);
                                     return event_loop_omp_task(100, 100, 100);
                                 };

    BENCHMARK("omp_task 8 100i-100x100"){
                                     omp_set_num_threads(8);
                                     return event_loop_omp_task(100, 100, 100);
                                 };
}

TEST_CASE("next state - 1000 iter, 100x100", "[1000-100-100]"){
    BENCHMARK("sequential 1000i-100x100"){
                               return event_loop(1000, 100, 100);
                           };
    BENCHMARK("omp 2 1000i-100x100"){
                                     omp_set_num_threads(2);
                                     return event_loop_omp(1000, 100, 100);
                                 };

    BENCHMARK("omp 4 1000i-100x100"){
                                     omp_set_num_threads(4);
                                     return event_loop_omp(1000, 100, 100);
                                 };

    BENCHMARK("omp 8 1000i-100x100"){
                                     omp_set_num_threads(8);
                                     return event_loop_omp(1000, 100, 100);
                                 };
    BENCHMARK("omp_task 2 1000i-100x100"){
                                     omp_set_num_threads(2);
                                     return event_loop_omp_task(1000, 100, 100);
                                 };

    BENCHMARK("omp_task 4 1000i-100x100"){
                                     omp_set_num_threads(4);
                                     return event_loop_omp_task(1000, 100, 100);
                                 };

    BENCHMARK("omp_task 8 1000i-100x100"){
                                     omp_set_num_threads(8);
                                     return event_loop_omp_task(1000, 100, 100);
                                 };
}

TEST_CASE("next state - 100 iter, 1000x1000", "[100-1000-1000]"){
    BENCHMARK("sequential 100i-1000x1000"){
                               return event_loop(100, 1000, 1000);
                           };
    BENCHMARK("omp 2 100i-1000x1000"){
                                     omp_set_num_threads(2);
                                     return event_loop_omp(100, 1000, 1000);
                                 };

    BENCHMARK("omp 4 100i-1000x1000"){
                                     omp_set_num_threads(4);
                                     return event_loop_omp(100, 1000, 1000);
                                 };

    BENCHMARK("omp 8 100i-1000x1000"){
                                     omp_set_num_threads(8);
                                     return event_loop_omp(100, 1000, 1000);
                                 };
    BENCHMARK("omp_task 2 100i-1000x1000"){
                                     omp_set_num_threads(2);
                                     return event_loop_omp_task(100, 1000, 1000);
                                 };

    BENCHMARK("omp_task 4 100i-1000x1000"){
                                     omp_set_num_threads(4);
                                     return event_loop_omp_task(100, 1000, 1000);
                                 };

    BENCHMARK("omp_task 8 100i-1000x1000"){
                                     omp_set_num_threads(8);
                                     return event_loop_omp_task(100, 1000, 1000);
                                 };
}
