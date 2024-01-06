# Conway's Game of Life

## Implementation
- Single threaded
- OpenMP
- MPI

## Benchmarks
Benchmarks are configured through 'Catch2' framework.

## Unit tests
Tests are configured through 'Catch2' framework.
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

## (run from cmake-build-release directory)
### Running a specific implementation 

#### Sequential && OpenMP Parallel For && OpenMP Parallel Task
```shell
./base [--openmp] [--openmpt]
```

#### MPI
```shell
mpiexec -np [number_of_processes] ./base --mpi
```

### Benchmarks 
#### Sequential && OpenMP
```shell
./benchmarks
```

#### MPI
```shell
mpiexec -np [number_of_processes] ./benchmarks-mpi
```

### Unit tests
```shell
./tests
```
