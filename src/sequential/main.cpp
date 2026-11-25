
#include <iostream>
#include <vector>
#include <cmath>
#include <chrono>
#include <algorithm>
#include <iomanip>
#include <limits>

using namespace std;

// Apply the matrix-free five-point operator A to vector p.
void apply_A(const vector<double>& p,
             vector<double>& q, int N) {

    fill(q.begin(), q.end(), 0.0);

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {

            int k = i * N + j;

            q[k] = 4.0 * p[k];

            if (i > 0)
                q[k] -= p[(i - 1) * N + j];

            if (i < N - 1)
                q[k] -= p[(i + 1) * N + j];

            if (j > 0)
                q[k] -= p[i * N + (j - 1)];

            if (j < N - 1)
                q[k] -= p[i * N + (j + 1)];
        }
    }
}

// Calculate dot product of two vectors.
double dot_product(const vector<double>& a,
                   const vector<double>& b) {

    double sum = 0.0;

    for (size_t i = 0; i < a.size(); i++)
        sum += a[i] * b[i];

    return sum;
}

// Calculate exact solution.
void generate_exact_solution(vector<double>& exact,
                             int N) {

    const double PI = acos(-1.0);

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {

            double x = double(i + 1) / (N + 1);
            double y = double(j + 1) / (N + 1);

            exact[i * N + j] =
                sin(PI * x) * sin(PI * y);
        }
    }
}

// Generate RHS using the same discrete operator.
void generate_rhs(const vector<double>& exact,
                  vector<double>& b, int N) {

    apply_A(exact, b, N);
}

// Sequential Conjugate Gradient solver.
struct SolverResult {
    int iterations;
    double relative_residual;
    double execution_time;
    bool converged;
};

double calculate_true_residual(
    const vector<double>& b,
    const vector<double>& x,
    int N
) {
    vector<double> Ax(N * N);

    apply_A(x, Ax, N);

    double residual_sum = 0.0;
    double b_sum = 0.0;

    for (size_t i = 0; i < b.size(); i++) {
        double r = b[i] - Ax[i];

        residual_sum += r * r;
        b_sum += b[i] * b[i];
    }

    return sqrt(residual_sum / b_sum);
}

SolverResult cg_solve(const vector<double>& b,
                      vector<double>& x,
                      int N, double tolerance,
                      int max_iterations) {

    int size = N * N;

    vector<double> r(size);
    vector<double> p(size);
    vector<double> q(size);

    x.assign(size, 0.0);

    // Initial residual: r = b - A*x = b
    r = b;
    p = r;

    double rr = dot_product(r, r);
    double b_norm = sqrt(rr);

    SolverResult result{0, 0.0, 0.0, false};

    if (b_norm == 0.0) {
        result.converged = true;
        return result;
    }

    auto start = chrono::steady_clock::now();

    double relative_residual = 1.0;

    for (int iter = 0;
         iter < max_iterations;
         iter++) {

        // q = A*p
        apply_A(p, q, N);

        // Calculate alpha
        double denominator = dot_product(p, q);

        if (denominator <= 0.0 ||
            !isfinite(denominator)) {
            break;
        }

        double alpha = rr / denominator;

        // Update solution and residual
        for (int i = 0; i < size; i++) {
            x[i] += alpha * p[i];
            r[i] -= alpha * q[i];
        }

        double rr_new = dot_product(r, r);

        relative_residual = sqrt(rr_new) / b_norm;

        result.iterations = iter + 1;
        result.relative_residual = relative_residual;

        if (relative_residual < tolerance) {
            result.converged = true;
            rr = rr_new;
            break;
        }

        if (rr_new <= 0.0)
            break;

        // Calculate beta
        double beta = rr_new / rr;

        // Update search direction
        for (int i = 0; i < size; i++)
            p[i] = r[i] + beta * p[i];

        rr = rr_new;
    }

    auto end = chrono::steady_clock::now();

    result.execution_time =
        chrono::duration<double>(end - start).count();

    result.relative_residual = relative_residual;

    return result;
}

// Calculate relative error against exact solution.
double calculate_error(const vector<double>& x,
                       const vector<double>& exact) {

    double error_sum = 0.0;
    double exact_sum = 0.0;

    for (size_t i = 0; i < x.size(); i++) {

        double difference = x[i] - exact[i];

        error_sum += difference * difference;
        exact_sum += exact[i] * exact[i];
    }

    return sqrt(error_sum / exact_sum);
}


int main(int argc, char* argv[]) 
{

    
bool csv_mode = false;
int N;

if (argc == 2) {
    N = stoi(argv[1]);
}
else if (argc == 3 && string(argv[1]) == "--csv") {
    csv_mode = true;
    N = stoi(argv[2]);
}
else {
    cerr << "Usage:\n";
    cerr << "./cg_solver <grid_size>\n";
    cerr << "./cg_solver --csv <grid_size>\n";
    return 1;
}


    if (N < 1 || N > 10000) {
        cerr << "Invalid grid size.\n";
        return 1;
    }

    int size = N * N;

    const double tolerance = 1e-8;
    int max_iterations = min(20 * N, size);

    vector<double> exact(size);
    vector<double> b(size);
    vector<double> x(size);

    generate_exact_solution(exact, N);
    generate_rhs(exact, b, N);

    SolverResult result =
        cg_solve(b, x, N, tolerance, max_iterations);

    double relative_error =
        calculate_error(x, exact);

    double true_residual =
        calculate_true_residual(b, x, N);

  

   

if (csv_mode) {

    cout << N << ","
         << result.iterations << ","
         << fixed << setprecision(9)
         << result.execution_time << ","
         << scientific << setprecision(6)
         << true_residual << ","
         << relative_error << ","
         << ((result.converged && true_residual < tolerance)
             ? "Yes" : "No")
         << "\n";

}
else {

    cout << "\n";
    cout << "====================================================\n";
    cout << "          SEQUENTIAL CONJUGATE GRADIENT SOLVER\n";
    cout << "====================================================\n";

    cout << left << setw(28) << "Grid Size:"
         << N << " x " << N << "\n";

    cout << left << setw(28) << "Total Unknowns:"
         << size << "\n";

    cout << left << setw(28) << "Maximum Iterations:"
         << max_iterations << "\n";

    cout << left << setw(28) << "Tolerance:"
         << scientific << tolerance << "\n";

    cout << "----------------------------------------------------\n";

    cout << left << setw(28) << "CG Iterations:"
         << result.iterations << "\n";

    cout << left << setw(28) << "Execution Time:"
         << fixed << setprecision(9)
         << result.execution_time << " seconds\n";

    cout << left << setw(28) << "Relative Residual:"
         << scientific << setprecision(6)
         << true_residual << "\n";

    cout << left << setw(28) << "Relative Error:"
         << relative_error << "\n";

    cout << "----------------------------------------------------\n";

    cout << left << setw(28) << "Convergence Status:"
         << ((result.converged && true_residual < tolerance)
             ? "SUCCESS" : "FAILED")
         << "\n";

    cout << "====================================================\n\n";
}

}