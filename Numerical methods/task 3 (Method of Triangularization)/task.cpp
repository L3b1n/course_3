#include <vector>
#include <numeric>
#include <iostream>

class LinearSystemSolver
{
public:
    LinearSystemSolver() : n(0) {}

    friend std::istream& operator>>(std::istream& in, LinearSystemSolver& a)
    {
        std::cout << "Enter N: ";
        in >> a.n;
        a.y.resize(a.n);
        a.f.resize(a.n, 0.0);
        a.A.resize(a.n, std::vector<float>(a.n));
        for(int i = 0; i < a.n; i++)
        {
            a.A[i][i] = a.m + a.k + i - 1;
            if(i > 0){ a.A[i][i - 1] = -a.k;}
            if(i + 1 < a.n){ a.A[i][i + 1] = a.m - 1 + i;}
        }
        a.A[0][0] = a.m;
        std::iota(a.y.begin(), a.y.end(), 1);
        for(int i = 0; i < a.n; i++) {
            for(int j = 0; j < a.n; j++){ a.f[i] += a.A[i][j] * a.y[j];}
        }
        return in;
    }

    void Solution()
    {
        auto [res1, t1] = solveTridiagonalSystem();
        auto [res2, t2] = gaussianElimination();
        std::cout << "Method of Triangularization\n";
        printResults(res1, t1); 
        std::cout << "Gaussian Elimination Method\n";
        printResults(res2, t2);
    }

private:
    int n;
    const int k = 3;
    const int m = 6;
    std::vector<float> y;
    std::vector<float> f;
    std::vector<std::vector<float>> A;

    // Gaussian elimination Ax = b
    std::pair<std::vector<float>, float> gaussianElimination() {
        auto start = std::chrono::high_resolution_clock::now();
        std::vector<std::vector<float>> augmentedMatrix(n, std::vector<float>(n + 1));

        // Create the augmented matrix from A and f
        for(int i = 0; i < n; i++)
        {
            for(int j = 0; j < n; j++){ augmentedMatrix[i][j] = A[i][j];}
            augmentedMatrix[i][n] = f[i];
        }

        // Forward move
        for(int i = 0; i < n; i++)
        {
            for (int k = i + 1; k < n; k++)
            {
                float l = augmentedMatrix[k][i] / augmentedMatrix[i][i];
                for(int j = i; j <= n; j++){ augmentedMatrix[k][j] -= l * augmentedMatrix[i][j];}
            }
        }

        // Back-substitution
        std::vector<float> x(n);
        for(int i = n - 1; i >= 0; i--)
        {
            x[i] = augmentedMatrix[i][n];
            for(int j = i + 1; j < n; j++){ x[i] -= augmentedMatrix[i][j] * x[j];}
            x[i] /= augmentedMatrix[i][i];
        }

        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<float> duration = end - start;

        return std::make_pair(x, duration.count());
    }

    std::pair<std::vector<float>, float> solveTridiagonalSystem() {
        auto start = std::chrono::high_resolution_clock::now();
        // Initialize vectors for the tridiagonal matrix and the result
        std::vector<float> a(n);  // lower diagonal
        std::vector<float> b(n);  // main diagonal
        std::vector<float> c(n);  // upper diagonal
        std::vector<float> res(n); // result vector

        // Fill vectors a, b, and c with the lower, main, and upper diagonals of matrix A
        for(int i = 0; i < n; i++)
        {
            b[i] = A[i][i]; // main diagonal
            res[i] = f[i];  // right-hand side vector

            // Check if there are elements in the lower and upper diagonals
            if(i > 0){ a[i] = A[i][i - 1];}
            if(i + 1 < n){ c[i] = A[i][i + 1];}
        }

        float temp;
        // Forward elimination: transform the matrix into an upper triangular form
        for(int i = 1; i < n; i++)
        {
            temp = a[i] / b[i - 1];
            b[i] = b[i] - temp * c[i - 1];
            res[i] = res[i] - temp * res[i - 1];
        }

        // Backward substitution: solve for the unknowns starting from the last row
        res[n - 1] = res[n - 1] / b[n - 1];
        for(int i = n - 2; i >= 0; i--){ res[i] = (res[i] - c[i] * res[i + 1]) / b[i];}

        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<float> duration = end - start;

        return std::make_pair(res, duration.count());
    }

    auto calculateRelativeError(const std::vector<float>& x,
                                const std::vector<float>& xPrecise) -> double {
        float normX = calculateNorm(xPrecise);
        std::vector<float> diff(x.size());
        for(auto i = 0; i < x.size(); ++i){ diff[i] = x[i] - xPrecise[i];}
        float normDiff = calculateNorm(diff);

        if(normX == 0.0){ return 0.0;}

        return normDiff / normX;
    }

    auto calculateNorm(const std::vector<float>& vector) ->float {
        float norm = 0.0;
        for (float value : vector) {
            norm += value * value;
        }
        return std::sqrt(norm);
    }

    void printResults(const std::vector<float>& x,
                      double time) {
        std::cout << "1) First 5 coordinates:\n\n(";
        for(int i = 0; i < 5; i++){ std::cout << x[i] << ", ";} std::cout << "...)\n\n";
        std::cout << "3) Relative error: " << calculateRelativeError(x, y) << "\n";
        std::cout << "4) Execution time: " << time << "\n\n";
    }
};

int main()
{
    std::ios_base::sync_with_stdio(0);
    std::cin.tie(0);
    std::cout.tie(0);
    LinearSystemSolver linearSystemSolver;
    std::cin >> linearSystemSolver;
    linearSystemSolver.Solution();
    return 0;
}