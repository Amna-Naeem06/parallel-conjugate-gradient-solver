
# Refined Parallelization Plan

## 1. Problem Overview

We solve a two-dimensional Poisson equation
using a matrix-free Conjugate Gradient solver.

The computational domain is divided into an
N x N grid.

Each CG iteration performs:
- Five-point stencil operation
- Dot products
- Vector updates

## 2. Sequential Baseline

The sequential implementation uses C++.

One thread performs all grid operations.

The matrix A is not stored. The five-point
stencil is applied directly.

The baseline execution times will be used
to calculate parallel speedup.

## 3. OpenMP Implementation

- Divide grid rows among threads.
- Use parallel for for stencil operations.
- Use reduction for dot products.
- Parallelize vector updates.
- Test multiple thread counts.

## 4. MPI Implementation

### 1D Decomposition
- Divide the grid into horizontal strips.
- Assign each strip to an MPI process.
- Use ghost rows for neighbouring values.
- Exchange boundaries using MPI_Isend
  and MPI_Irecv.
- Use MPI_Allreduce for global dot products.

### 2D Decomposition
- Divide the grid into rectangular blocks.
- Use MPI_Cart_create for process topology.
- Exchange boundary rows and columns.
- Compare communication overhead with 1D.

## 5. Hybrid MPI + OpenMP

- Use MPI processes for domain decomposition.
- Use OpenMP threads inside each process.
- Reduce communication between processes.
- Compare against pure MPI using equal
  total core counts.

## 6. CUDA Implementation

- Assign GPU threads to grid points.
- Implement stencil computation as a kernel.
- Implement vector updates on GPU.
- Use reductions for dot products.
- Keep vectors on GPU throughout the solve.
- Compare single and double precision.

## 7. Performance Metrics

- Execution time
- Speedup
- Parallel efficiency
- Iteration count
- Relative residual
- Numerical error
- Communication overhead

## 8. Expected Challenges

- Memory bandwidth limitations
- Synchronization overhead
- MPI communication costs
- Floating-point differences
- GPU memory management

## 9. Evaluation Strategy

All parallel implementations will be compared
against the sequential baseline using the same
grid sizes, mathematical problem and tolerance.
