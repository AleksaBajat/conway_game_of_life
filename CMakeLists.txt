cmake_minimum_required(VERSION 3.27)
project(base)
set(CMAKE_CXX_STANDARD 17)

find_package(OpenMP REQUIRED)
find_package(MPI REQUIRED)
add_subdirectory(lib/Catch2)

add_executable(base main.cpp
        include/state.h
        src/state.cpp
)
target_include_directories(base PRIVATE include)
target_link_libraries(base PRIVATE MPI::MPI_CXX)

if(OpenMP_CXX_FOUND)
    target_compile_options(base PUBLIC ${OpenMP_CXX_FLAGS})
    target_link_libraries(base PUBLIC OpenMP::OpenMP_CXX)
endif()
#else()
#    MESSAGE(SEND_ERROR "no openmp")
#endif()

add_executable(tests tests/test_state.cpp src/state.cpp)
target_include_directories(tests PRIVATE include)

add_executable(benchmarks benchmarks/benchmark.cpp src/state.cpp)
target_compile_definitions(benchmarks PRIVATE BENCH=1)
target_include_directories(benchmarks PRIVATE include)
target_link_libraries(benchmarks PRIVATE MPI::MPI_CXX)

if(OpenMP_CXX_FOUND)
    target_compile_options(benchmarks PUBLIC ${OpenMP_CXX_FLAGS})
    target_link_libraries(benchmarks PUBLIC OpenMP::OpenMP_CXX)
endif()

target_link_libraries(benchmarks PRIVATE Catch2::Catch2WithMain)
target_link_libraries(tests PRIVATE Catch2::Catch2WithMain)
include(CTest)
include(Catch)
catch_discover_tests(tests)