# Conway's Game of Life

## Implementation
- Single threaded
- OpenMP
- MPI

## Benchmarks
Benchmarks for single threaded and OpenMP implementations are configured through 'Catch2' framework, while MPI due to it's nature to completely change the way the code is being written and executed is benchmarked through ```std::chrono::system_clock```.

## Tests
- Next Board State
- Number of Node Neighbours

## How to build? 

### Setup (run from main.cpp directory)
```shell
make setup
```

```shell
make build
```

### Running a (run from cmake-build-release directory)
#### Tests
```shell
./tests
```

#### Benchmarks (Single thread && OpenMP) 
```shell
./benchmarks
```

#### MPI 
```shell
mpiexec -np [process_num] base
```
