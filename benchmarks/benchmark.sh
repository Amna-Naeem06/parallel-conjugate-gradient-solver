
#!/bin/bash

set -e

mkdir -p results

echo "N,iterations,time_seconds,relative_residual,relative_error,converged" > results/baseline_results.csv

for N in 128 256 512
do
    echo "Benchmarking grid: ${N} x ${N}"

    for run in {1..5}
    do
        ./cg_solver --csv "$N" >> results/baseline_results.csv
    done
done

echo "Benchmark completed!"
