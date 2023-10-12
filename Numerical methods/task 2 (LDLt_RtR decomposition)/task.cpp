#include <ctime>
#include <vector>
#include <random>
#include <chrono>
#include <numeric>
#include <iomanip>
#include <iostream>
#include <algorithm>

class LDLt_RtR
{
public:
    LDLt_RtR() : n(0) {}

    friend std::istream& operator >> (std::istream& in, LDLt_RtR& a) {
        std::cout << "Enter n: ";
        in >> a.n;

        a.A.resize(a.n, std::vector<float>(a.n, 0.0));
        std::mt19937 rng(static_cast<unsigned int>(std::time(nullptr)));
        std::uniform_real_distribution<float> dist(0.0, 1000.0);

        // Generate random values for the upper triangular part
        for(int i = 0; i < a.n; ++i)
        {
            for(int j = i; j < a.n; ++j)
            {
                if(i == j){
                    for(int k = 0; k < a.n; ++k){ if(i != k){ a.A[i][j] += a.A[i][k];}}
                    a.A[i][j] *= -1;
                } else {
                    a.A[i][j] = dist(rng);
                    a.A[j][i] = a.A[i][j];
                }
            }
        }

        a.A[0][0] = float(1e-1) - std::accumulate(a.A[0].begin() + 1, a.A[0].end(), 0.0);

        // generate vector x = (6, 7, 8, ..., 6 + n - 1)
        a.x.resize(a.n);
        std::iota(a.x.begin(), a.x.end(), 6.0);
        
        // Function to calculate b = A * x  
        int i = 0;
        a.b.resize(a.n, 0.0);
        std::for_each(a.A.begin(), a.A.end(),
            [&i, &a](std::vector<float> vec){
                int j = 0;
                std::for_each(vec.begin(), vec.end(),
                    [&j, &i, &a](float temp){
                        a.b[i] += temp * a.x[j]; j++;
                    });
                i++;
            });
        return in;
    }

    void Solution() {
        auto [result1, time1] = gaussianElimination();
        auto [result2, time2] = gaussianEliminationPivot();
        auto [result3, time3] = ldltSolve();

        std::cout << std::fixed << std::setprecision(8);
        std::cout << "Gaussian elimination: \n";
        printResults(x, result1, b, time1, false);

        std::cout << "\nGaussian Pivot elimination: \n";
        printResults(x, result2, b, time2, false);

        std::cout << "\nLDLT Decomposition: \n";
        printResults(x, result3, b, time3, true);
    }

private:
    int n;
    std::vector<float> x;
    std::vector<float> b;
    std::vector<std::vector<float>> A;

    float calculateNorm(const std::vector<float>& vector) {
        float norm = 0.0;
        std::for_each(vector.begin(), vector.end(),
            [&norm](const float& tempValue){
                norm += tempValue * tempValue;
            });
        return std::sqrt(norm);
    }

    float calculateResidualNorm() {
        int i = 0;
        std::vector<float> residual(n, 0.0);
        std::for_each(residual.begin(), residual.end(),
            [this, &i](float& temp){
                int j = 0;
                std::for_each(x.begin(), x.end(),
                    [this, &i, &j, &temp](const float& tempX){
                        temp += A[i][j++] * tempX;
                    });
                temp = b[i++] - temp;
            });
        return calculateNorm(residual);
    }
    
    auto calculateRelativeError(const std::vector<float>& x,
                                const std::vector<float>& xPrecise) -> float {
        int i = 0;
        std::vector<float> diff(n);
        float normX = calculateNorm(xPrecise);
        std::for_each(diff.begin(), diff.end(),
            [&i, &x, &xPrecise](float& temp){
                temp = x[i] - xPrecise[i]; i++;
            });
        float normDiff = calculateNorm(diff);
        if(normX == 0.0){ return 0.0;}
        return normDiff / normX;
    }

    // Gaussian elimination Ax = b
    std::pair<std::vector<float>, float> gaussianElimination() {
        std::chrono::steady_clock::time_point start = std::chrono::high_resolution_clock::now();
        std::vector<std::vector<float>> augmentedMatrix(n, std::vector<float>(n + 1));

        // Create the augmented matrix from A and b
        int i = 0;
        std::for_each(A.begin(), A.end(),
            [this, &i, &augmentedMatrix](std::vector<float>& vec){
                int j = 0;
                std::copy(vec.begin(), vec.end(), std::inserter(augmentedMatrix[i], augmentedMatrix[i].begin() + j++));
                augmentedMatrix[i][n] = b[i]; i++;
            });

        // Forward move
        for(int i = 0; i < n; i++)
        {
            for(int k = i + 1; k < n; k++)
            {
                float l = augmentedMatrix[k][i] / augmentedMatrix[i][i];
                for(int j = i; j <= n; j++){ augmentedMatrix[k][j] -= l * augmentedMatrix[i][j];}
            }
        }

        // Back-substitution
        std::vector<float> X(n);
        for(int i = n - 1; i >= 0; i--)
        {
            X[i] = augmentedMatrix[i][n];
            for(int j = i + 1; j < n; j++){ X[i] -= augmentedMatrix[i][j] * X[j];}
            X[i] /= augmentedMatrix[i][i];
        }

        std::chrono::steady_clock::time_point end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<float> duration = end - start;

        return std::make_pair(X, duration.count());
    }

    // Gaussian Pivot elimination Ax = b
    std::pair<std::vector<float>, float> gaussianEliminationPivot() {
        std::chrono::steady_clock::time_point start = std::chrono::high_resolution_clock::now();
        std::vector<std::vector<float>> augmentedMatrix(n, std::vector<float>(n + 1));

        // Create an augmented matrix from A and b
        int i = 0;
        std::for_each(A.begin(), A.end(),
            [this, &i, &augmentedMatrix](std::vector<float>& vec){
                int j = 0;
                std::copy(vec.begin(), vec.end(), std::inserter(augmentedMatrix[i], augmentedMatrix[i].begin() + j++));
                augmentedMatrix[i][n] = b[i]; i++;
            });

        // Perform Gaussian elimination
        for(int i = 0; i < n; i++)
        {
            // Pivotization
            for(int k = i + 1; k < n; k++)
            {
                if(std::abs(augmentedMatrix[i][i]) < std::abs(augmentedMatrix[k][i])){
                    for(int j = 0; j <= n; j++){ std::swap(augmentedMatrix[i][j], augmentedMatrix[k][j]);}
                }
            }

            // Elimination
            for(int k = i + 1; k < n; k++)
            {
                int j = i;
                float l = augmentedMatrix[k][i] / augmentedMatrix[i][i];
                std::for_each(augmentedMatrix[k].begin() + i, augmentedMatrix[k].end(),
                    [&i, &l, &j, &augmentedMatrix](float& temp){
                        temp -= l * augmentedMatrix[i][j++];
                    });
            }
        }

        // Back-substitution
        i = n - 1;
        std::vector<float> X(n);
        std::for_each(X.rbegin(), X.rend(),
            [this, &i, &augmentedMatrix, &X](float& temp){
                temp = augmentedMatrix[i][n];
                for(int j = i + 1; j < n; j++){ temp -= augmentedMatrix[i][j] * X[j];}
                temp /= augmentedMatrix[i][i]; i--;
            });

        std::chrono::steady_clock::time_point end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<float> duration = end - start;
        return std::make_pair(X, duration.count());
    }

    // Create L and D
    void ldltDecomposition(std::vector<std::vector<float>>& L,
                           std::vector<float>& D) {
        D.resize(n, 0.0);
        L.resize(n, std::vector<float>(n, 0.0));

        for(int i = 0; i < n; ++i)
        {
            for(int j = 0; j <= i; ++j)
            {
                float sum = A[i][j];
                for(int k = 0; k < i; ++k){ sum -= L[i][k] * D[k] * L[j][k];}
                if(i == j){
                    D[i] = sum;
                    L[i][i] = 1.0;
                } else {
                    L[i][j] = sum / D[j];
                }
            }
        }
    }

    // Function to solve the system of linear equations Ax = b using LDLT decomposition
    std::pair<std::vector<float>, float> ldltSolve() {
        std::chrono::steady_clock::time_point start = std::chrono::high_resolution_clock::now();
        std::vector<float> D;
        std::vector<std::vector<float>> L;

        // Perform LDLT decomposition
        ldltDecomposition(L, D);

        // Solve Ly = b
        int i = 0;
        std::vector<float> y(n, 0.0);
        for(int i = 0; i < n; ++i)
        {
            y[i] = b[i];
            for(int j = 0; j < i; ++j){ y[i] -= L[i][j] * y[j];}
        }

        // Solve Dz = y
        std::vector<float> z(n, 0.0);
        for (int i = 0; i < n; ++i) {
            z[i] = y[i] / D[i];
        }

        // Solve L^Tx = z
        std::vector<float> X(n, 0.0);
        for(int i = n - 1; i >= 0; --i)
        {
            X[i] = z[i];
            for(int j = i + 1; j < n; ++j){ X[i] -= L[j][i] * X[j];}
        }

        std::chrono::steady_clock::time_point end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<float> duration = end - start;

        return std::make_pair(X, duration.count());
    }

    void printResults(const std::vector<float>& xPrecise,
                      const std::vector<float>& x,
                      const std::vector<float>& b,
                      double time,
                      bool isLDLT) {
        std::cout << "1) First 5 coordinates:\n\n(";
        for(int i = 0; i < 5; i++){ std::cout << x[i] << ", ";} std::cout << "...)\n\n";
        if(!isLDLT){ std::cout << "2) Residual norm: " << calculateResidualNorm() << "\n";}
        std::cout << "3) Relative error: " << calculateRelativeError(x, xPrecise) << "\n";
        std::cout << "4) Execution time: " << time << "\n";
    }
};

int main()
{
    std::ios_base::sync_with_stdio(0);
    std::cin.tie(0);
    std::cout.tie(0);
    LDLt_RtR lDLt_RtR;
    std::cin >> lDLt_RtR;
    lDLt_RtR.Solution();
    return 0;
}
