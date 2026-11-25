# Correctness Testing

## Test Problem
Two-dimensional Poisson equation.

## Exact Solution
u(x,y) = sin(pi*x) * sin(pi*y)

## Boundary Conditions
Zero Dirichlet boundary conditions.

## Numerical Method
Conjugate Gradient.

## Operator
Five-point finite difference stencil.

## Tolerance
1e-8

## Validation Metrics
- Relative residual
- Relative numerical error
- Convergence status

## Test Cases
- 16 x 16
- 32 x 32
- 64 x 64
- 128 x 128

## Acceptance Criteria
The solver must converge within the specified
tolerance and produce a small relative error.