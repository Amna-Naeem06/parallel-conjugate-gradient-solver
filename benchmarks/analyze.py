
import csv
import statistics
import matplotlib.pyplot as plt

filename = "results/baseline_results.csv"

data = {}

with open(filename, "r") as file:
    reader = csv.DictReader(file)

    for row in reader:
        n = int(row["N"])
        time = float(row["time_seconds"])

        data.setdefault(n, []).append(time)

print("\nSEQUENTIAL BASELINE RESULTS")
print("-" * 55)

grid_sizes = []
median_times = []

for n in sorted(data):
    times = data[n]
    median_time = statistics.median(times)

    grid_sizes.append(n)
    median_times.append(median_time)

    print(f"Grid: {n} x {n}")
    print(f"Runs: {len(times)}")
    print(f"Median time: {median_time:.6f} seconds")
    print("-" * 55)

plt.figure(figsize=(8, 5))

plt.plot(
    grid_sizes,
    median_times,
    marker="o",
    label="Sequential CG"
)

plt.xlabel("Grid size (N)")
plt.ylabel("Execution time (seconds)")
plt.title("Sequential CG Solver Performance")
plt.grid(True)
plt.legend()

plt.savefig("results/sequential_performance.png",
            dpi=300, bbox_inches="tight")

plt.show()
