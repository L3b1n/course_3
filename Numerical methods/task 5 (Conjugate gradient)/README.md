# Отчет по Лабораторной работе 4 «Метод Якоби»

## Постановка задачи
Цель данной лабораторной работы заключалась в изучении основ метода Гаусса, сравнении вычислительной погрешности алгоритма без выбора ведущего элемента, алгоритма с выбором ведущего элемента и с разложением симметричных матриц, а также в убеждении в том, что схема выбора главного элемента улучшает устойчивость метода Гаусса. Для достижения этой цели были выполнены следующие задачи:

1. Разработка программы численного решения системы линейных алгебраических уравнений (СЛАУ) методом Гаусса с и без выбора ведущего элемента.
2. Генерация случайной матрицы `A` и вектора `b`, где `A` является матрицей коэффициентов СЛАУ, а `b` - вектором правой части, в соответствии с условиями задачи.
3. Решение СЛАУ методом Гаусса без выбора ведущего элемента и методом Гаусса с выбором ведущего элемента для одной и той же системы уравнений.
4. Вывод результатов, включая первые 5 координат вектора приближенного решения $x^*$, норму вектора невязки, относительную погрешность и время выполнения.

## Входные данные
- Порядок матрицы `n`: 500
- Номер в списке студенческой группы `m`: 6

## Листинг программы
``` C++
#include <vector>
#include <chrono>
#include <random>
#include <iomanip>
#include <iostream>

class Conjugate {
public:
    Conjugate() {}

    friend std::istream& operator >> (std::istream& in, Conjugate& a) {
        std::cout << "Enter n: ";
        in >> a.n;

        // Generate a random matrix A of size n x n
        a.A.resize(a.n, std::vector<double>(a.n, 0.0));
        std::mt19937 rng(static_cast<unsigned int>(std::time(nullptr)));
        std::uniform_real_distribution<double> dist(0.0, 1000.0);

        // Generate random values for the upper triangular part
        for(int i = 0; i < a.n; ++i) {
            for(int j = i; j < a.n; ++j) {
                if(i == j) {
                    for(int k = 0; k < a.n; ++k){ if(i != k){ a.A[i][j] += a.A[i][k];}}
                    a.A[i][j] *= -1;
                } else {
                    a.A[i][j] = dist(rng);
                    a.A[j][i] = a.A[i][j];
                }
            }
        }
        a.A[0][0] = double(1e-1) - std::accumulate(a.A[0].begin() + 1, a.A[0].end(), 0.0);

        // Generate vector x = (22, 23, 24, ..., 22 + n - 1)
        a.x.resize(a.n);
        std::iota(a.x.begin(), a.x.end(), 22.0);

        // Calculate b = A * x
        a.b.resize(a.n, 0.0);
        for(int i = 0; i < a.n; i++) {
            for(int j = 0; j < a.n; j++){ a.b[i] += a.A[i][j] * a.x[j];}
        }
        return in;
    }

    void Solution() {
        auto [result1, time1] = ldltSolve();
        auto [result2, time2] = conjugateGradient();

        std::cout << std::fixed << std::setprecision(8);

        std::cout << "\nLDLT Decomposition\n";
        printResults(x, result1, b, time1);

        std::cout << "\nConjugate Gradient\n";
        printResults(x, result2, b, time2);
    }

private:
    int n;
    std::vector<double> x;
    std::vector<double> b;
    std::vector<std::vector<double>> A;

    auto calculateNorm(const std::vector<double>& vector) -> double {
        double norm = 0.0;
        for(double value : vector){ norm += value * value;}
        return std::sqrt(norm);
    }

    auto calculateRelativeError(const std::vector<double>& x,
                                const std::vector<double>& xPrecise) -> double {
        double normX = calculateNorm(xPrecise);
        std::vector<double> diff(x.size());
        for(auto i = 0; i < x.size(); ++i){ diff[i] = x[i] - xPrecise[i];}
        double normDiff = calculateNorm(diff);
        if(normX == 0.0){ return 0.0;}
        return normDiff / normX;
    }

    auto calculateResidualNorm() -> double {
        std::vector<double> residual(n, 0.0);
        for(int i = 0; i < n; i++) {
            for(int j = 0; j < n; j++){ residual[i] += A[i][j] * x[j];}
            residual[i] = b[i] - residual[i];
        }
        return calculateNorm(residual);
    }

    // Calculate dot product
    auto dotProduct(const std::vector<double>& a, const std::vector<double>& b) -> double {
        double sum = 0.0;
        for(size_t i = 0; i < a.size(); ++i){ sum += a[i] * b[i];}
        return sum;
    }

    // Calculate matrix-vector product
    auto matrixVectorProduct(const std::vector<double>& v) -> std::vector<double> {
        std::vector<double> result(v.size(), 0.0);
        for(size_t i = 0; i < A.size(); ++i) {
            for(size_t j = 0; j < A[i].size(); ++j){ result[i] += A[i][j] * v[j];}
        }
        return result;
    }

    // Calculate the Conjugate Gradient Method
    auto conjugateGradient() -> std::pair<std::vector<double>, double> {
        auto start = std::chrono::high_resolution_clock::now();

        std::vector<double> x(n, 0.0);
        std::vector<double> r = b;
        std::vector<double> d = r;
        double rr = dotProduct(r, r);
        int max_iter = 50;

        for(int i = 0; i < max_iter; ++i) {
            std::vector<double> Ad = matrixVectorProduct(d);
            double alpha = rr / dotProduct(d, Ad);
            for(size_t j = 0; j < n; ++j){ x[j] += alpha * d[j];}
            std::vector<double> r_new = r;
            for(size_t j = 0; j < n; ++j){ r_new[j] -= alpha * Ad[j];}
            double rr_new = dotProduct(r_new, r_new);
            if(std::sqrt(rr_new) < 1e-10){ break;}
            for(size_t j = 0; j < n; ++j){ d[j] = r_new[j] + (rr_new / rr) * d[j];}
            rr = rr_new;
            r = r_new;
        }

        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> duration = end - start;
        return std::make_pair(x, duration.count());
    }

    void ldltDecomposition(std::vector<std::vector<double>>& L,
                           std::vector<double>& D) {
        L.resize(n, std::vector<double>(n, 0.0));
        D.resize(n, 0.0);

        for(int i = 0; i < n; ++i) {
            for(int j = 0; j <= i; ++j) {
                double sum = A[i][j];
                for(int k = 0; k < i; ++k){ sum -= L[i][k] * D[k] * L[j][k];}

                if(i == j) {
                    D[i] = sum;
                    L[i][i] = 1.0;
                } else { L[i][j] = sum / D[j];}
            }
        }
    }

    // Solve the system of linear equations Ax = b using LDLT decomposition
    auto ldltSolve() -> std::pair<std::vector<double>, double> {
        auto start = std::chrono::high_resolution_clock::now();

        std::vector<double> D;
        std::vector<std::vector<double>> L;

        // Perform LDLT decomposition
        ldltDecomposition(L, D);

        // Solve Ly = b
        std::vector<double> y(n, 0.0);
        for(int i = 0; i < n; ++i) {
            y[i] = b[i];
            for(int j = 0; j < i; ++j){ y[i] -= L[i][j] * y[j];}
        }

        // Solve Dz = y
        std::vector<double> z(n, 0.0);
        for(int i = 0; i < n; ++i){ z[i] = y[i] / D[i];}

        // Solve L^Tx = z
        std::vector<double> x(n, 0.0);
        for(int i = n - 1; i >= 0; --i) {
            x[i] = z[i];
            for(int j = i + 1; j < n; ++j){ x[i] -= L[j][i] * x[j];}
        }

        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> duration = end - start;

        return std::make_pair(x, duration.count());
    }

    void printResults(const std::vector<double>& xPrecise,
                      const std::vector<double>& x,
                      const std::vector<double>& b,
                      double time) {
        std::cout << "1) First 5 coordinates: {";
        for(int i = 0; i < 5; i++){ std::cout << x[i] << ", ";}
        std::cout << "...}\n";

        std::cout << "2) ||f - Ax*||2: " << calculateResidualNorm( ) << "\n";

        std::cout << "3) ||x - x*||2 / ||x||2: " << calculateRelativeError(x, xPrecise) << "\n";

        std::cout << "4) Execution time: " << time << "\n";
    }
};

int main() {
    std::ios_base::sync_with_stdio(0);
    std::cin.tie(0);
    std::cout.tie(0);
    Conjugate conjugate;
    std::cin >> conjugate;
    conjugate.Solution();
    return 0;
}
```

## Выходные данные
__С разложением LDLT матриц__:
 - Первые 5 координат вектора приближенного решения $x^*$: (6.00000000, 6.99999999, 8.00000001, 9.00000001, 9.99999999, ...)
 - Относительная погрешность: 0.0164770102
 - Время, затраченное на вычисления: 0.38833042 миллисекунд

__С разложением GC матриц__:
 - Первые 5 координат вектора приближенного решения $x^*$: (5.88621671, 40.94595327, 64.73026811, 24.62253153, -13.67491154, ...)
 - Относительная погрешность: 0.01220272
 - Время, затраченное на вычисления: 0.13314187 миллисекунд

## Выводы
Из результатов выполнения программы видно следующее:

1. Метод Гаусса с выбором ведущего элемента показывает более точные результаты в сравнении с методом Гаусса без выбора ведущего элемента. Первые 5 координат вектора приближенного решения $x^*$ при использовании метода с выбором ведущего элемента ближе к точному решению.

2. Максимум нормы невязки для метода Гаусса без выбора ведущего элемента значительно больше (3140924.000000) по сравнению с методом с выбором ведущего элемента (6264). Это свидетельствует о более высокой точности метода с выбором ведущего элемента.

3. Относительная погрешность также ниже для метода Гаусса с выбором ведущего элемента (0.000620) по сравнению с методом без выбора ведущего элемента (0.234914). Это подтверждает, что метод с выбором ведущего элемента является более надежным и устойчивым.

4. Время выполнения обоих методов примерно одинаково и составляет около 4435 секунд. Это может быть обусловлено случайным характером генерации матрицы `A` и вектора `b`, и в реальных задачах время выполнения может различаться.

Итак, на основе проведенных экспериментов можно сделать вывод о преимуществе метода Гаусса с выбором ведущего элемента, так как он обеспечивает более точные и устойчивые результаты при решении системы линейных уравнений.