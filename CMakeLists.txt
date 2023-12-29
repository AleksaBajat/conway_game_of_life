cmake_minimum_required(VERSION 3.27)
project(base)
set(CMAKE_CXX_STANDARD 17)


add_subdirectory(lib/Catch2)
find_package(OpenMP REQUIRED)

add_executable(base main.cpp
        include/state.h
        src/state.cpp
        test.cpp)


if (OpenMP_C_FOUND)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    target_link_libraries(base PRIVATE ${OpenMP_C_LIBRARIES})
endif ()

target_link_libraries(base PRIVATE Catch2::Catch2WithMain)
include(CTest)
include(Catch)