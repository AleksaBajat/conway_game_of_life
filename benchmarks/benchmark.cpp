#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>
#include "catch2/benchmark/catch_benchmark.hpp"
#include "../include/state.h"

TEST_CASE("next state - 100", "[100]"){
    BENCHMARK("next_state"){
                               return event_loop(100);
                           };
    BENCHMARK("next_state_omp 2"){
                                     omp_set_num_threads(2);
                                     return event_loop_omp(100);
                                 };

    BENCHMARK("next_state_omp 4"){
                                     omp_set_num_threads(4);
                                     return event_loop_omp(100);
                                 };

    BENCHMARK("next_state_omp 8"){
                                     omp_set_num_threads(8);
                                     return event_loop_omp(100);
                                 };
    BENCHMARK("next_state_omp 12"){
                                     omp_set_num_threads(12);
                                     return event_loop_omp(100);
                                 };
}

TEST_CASE("next state - 1000", "[1000]"){
    BENCHMARK("next_state"){
                               return event_loop(1000);
                           };
    BENCHMARK("next_state_omp 2"){
                                     omp_set_num_threads(2);
                                     return event_loop_omp(1000);
                                 };

    BENCHMARK("next_state_omp 4"){
                                     omp_set_num_threads(4);
                                     return event_loop_omp(1000);
                                 };

    BENCHMARK("next_state_omp 8"){
                                     omp_set_num_threads(8);
                                     return event_loop_omp(1000);
                                 };
    BENCHMARK("next_state_omp 12"){
                                      omp_set_num_threads(12);
                                      return event_loop_omp(1000);
                                  };
}

TEST_CASE("next state - 10000", "[10000]"){
    BENCHMARK("next_state"){
                               return event_loop(10000);
                           };
    BENCHMARK("next_state_omp 2"){
                                     omp_set_num_threads(2);
                                     return event_loop_omp(10000);
                                 };

    BENCHMARK("next_state_omp 4"){
                                     omp_set_num_threads(4);
                                     return event_loop_omp(10000);
                                 };

    BENCHMARK("next_state_omp 8"){
                                     omp_set_num_threads(8);
                                     return event_loop_omp(10000);
                                 };
    BENCHMARK("next_state_omp 12"){
                                      omp_set_num_threads(12);
                                      return event_loop_omp(10000);
                                  };
}
