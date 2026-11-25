
# Parallel Conjugate Gradient Solver

## Project Overview

A matrix-free Conjugate Gradient solver for
calculating steady-state temperature distribution
across a two-dimensional metal plate.

Course: CS426 - Parallel and Distributed Computing

## Phase 1: Sequential Baseline

Implementation: C++

Numerical Method: Conjugate Gradient

Discretization: Five-Point Finite Difference Stencil

Matrix Storage: Matrix-Free

Tolerance: 1e-8

## Requirements

- Ubuntu Linux
- G++ compiler
- C++17

## Build Instructions

From the project root:

g++ -O2 -std=c++17 -Wall -Wextra -pedantic \
src/sequential/main.cpp -o cg_solver

## Run Instructions

./cg_solver 128

./cg_solver 256

./cg_solver 512

## Correctness Validation

The solver is tested using:

u(x,y) = sin(pi*x) sin(pi*y)

Validation metrics:
- Relative residual
- Relative error
- Convergence status

## Benchmarking

Run:

chmod +x benchmarks/benchmark.sh

./benchmarks/benchmark.sh

Analyze:

python3 benchmarks/analyze.py

## Results

Results are stored in:

results/baseline_results.csv

The sequential performance graph is stored in:

results/sequential_performance.png

## Future Implementations

- OpenMP
- MPI 1D and 2D
- Hybrid MPI + OpenMP
- CUDA
