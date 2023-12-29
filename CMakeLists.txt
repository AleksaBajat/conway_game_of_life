cmake_minimum_required(VERSION 3.27)
project(base)
set(CMAKE_CXX_STANDARD 17)

find_package(OpenMP REQUIRED)
add_subdirectory(lib/Catch2)

add_executable(base main.cpp
        include/state.h
        src/state.cpp)
target_include_directories(base PRIVATE include)

if (OpenMP_C_FOUND)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    target_link_libraries(base PRIVATE ${OpenMP_C_LIBRARIES})
endif ()

add_executable(tests tests/test_state.cpp src/state.cpp)
target_include_directories(tests PRIVATE include)

target_link_libraries(tests PRIVATE Catch2::Catch2WithMain)
include(CTest)
include(Catch)
catch_discover_tests(tests)