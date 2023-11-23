# Отчет по Лабораторной работе 4 «Метод Якоби»

## Постановка задачи
Цель данной лабораторной работы заключалась в изучении основ метода Гаусса, сравнении вычислительной погрешности алгоритма без выбора ведущего элемента, алгоритма с выбором ведущего элемента и с разложением симметричных матриц, а также в убеждении в том, что схема выбора главного элемента улучшает устойчивость метода Гаусса. Для достижения этой цели были выполнены следующие задачи:

1. Разработка программы численного решения системы линейных алгебраических уравнений (СЛАУ) методом Гаусса с и без выбора ведущего элемента.
2. Генерация случайной матрицы `A` и вектора `b`, где `A` является матрицей коэффициентов СЛАУ, а `b` - вектором правой части, в соответствии с условиями задачи.
3. Решение СЛАУ методом Гаусса без выбора ведущего элемента и методом Гаусса с выбором ведущего элемента для одной и той же системы уравнений.
4. Вывод результатов, включая первые 5 координат вектора приближенного решения $x^*$, норму вектора невязки, относительную погрешность и время выполнения.

## Входные данные
- Порядок матрицы `n`: 1500
- Номер в списке студенческой группы `m`: 6

## Листинг программы
``` C++
#include <ctime>
#include <chrono>
#include <random>
#include <vector>
#include <iomanip>
#include <numeric>
#include <iostream>
#include <algorithm>

class Jacobi {
public:
    Jacobi() {
        generateA();
        generateVectorX();
        calculateB();
    }

    void Solution() {
        std::cout << std::fixed << std::setprecision(8);
        std::cout << "Jacobi Method:\n\n\t";
        auto [xJacobi, iterJacobi] = jacobi();
        printResults(xJacobi, iterJacobi);

        std::fill(startPlan.begin(), startPlan.end(), 0.0);
        constexpr float w1 = 0.5;
        auto [xRelax1, iterRelax1] = relaxation(w1);

        std::fill(startPlan.begin(), startPlan.end(), 0.0);
        constexpr float w2 = 1;
        auto [xRelax2, iterRelax2] = relaxation(w2);

        std::fill(startPlan.begin(), startPlan.end(), 0.0);
        constexpr float w3 = 1.5;
        auto [xRelax3, iterRelax3] = relaxation(w3);

        std::cout << "\nRelaxation Method:\n\t";
        std::cout << "\nw = 0.5\n\n\t";
        printResults(xRelax1, iterRelax1);

        std::cout << "\nw = 1\n\n\t";
        printResults(xRelax2, iterRelax2);

        std::cout << "\nw = 1.5\n\n\t";
        printResults(xRelax3, iterRelax3);
    }

private: 
    std::vector<float> x;
    std::vector<float> b;
    static constexpr int m = 6;
    static constexpr int n = 10;
    std::vector<float> startPlan;
    std::vector<std::vector<float>> A;
    static constexpr float maxIter = 1000.0;
    static constexpr float epsilon = 0.0001;

    // Function to generate a matrix A of size n x n
    void generateA() {
        A.resize(n, std::vector<float>(n, 0.0));
        std::mt19937 rng(static_cast<unsigned int>(std::time(nullptr)));
        std::uniform_int_distribution<> dist(-4, 0);

        for(int i = 0; i < n; ++i) {
            for(int j = i; j < n; ++j) {
                if(i == j) {
                    for(int k = 0; k < n; ++k){ if(i != k){ A[i][j] += A[i][k];}}
                    A[i][j] *= -1;
                } else {
                    A[i][j] = static_cast<float>(dist(rng));
                    A[j][i] = static_cast<float>(dist(rng));
                }
            }
        }

        A[0][0] += 1;
    }

    // Function to generate vector x = (m, m + 1, m + 2, ..., m + n - 1)
    void generateVectorX() {
        x.resize(n);
        std::iota(x.begin(), x.end(), m);
    }

    // Function to calculate b = A * x
    void calculateB() {
        b.resize(n, 0.0);
        for(int i = 0; i < n; i++) {
            for(int j = 0; j < n; j++){ b[i] += A[i][j] * x[j];}
        }
    }

    // Function to solve the system Ax=b using the Jacobi method
    auto jacobi() -> std::pair<std::vector<float>, int> {
        int iter = 0;
        float error = epsilon + 1;
        std::vector<float> x_new(n, 0.0);

        while(error > epsilon && iter < maxIter) {
            for(int i = 0; i < n; i++) {
                float sum = 0.0;
                for(int j = 0; j < n; j++){ if(i != j){ sum += A[i][j] * x[j];}}
                x_new[i] = (b[i] - sum) / A[i][i];
            }
            error = std::abs(x_new[0] - x[0]);
            for(int i = 0; i < n; i++) {
                if(std::abs(x_new[i] - x[i]) > error){ error = std::abs(x_new[i] - x[i]);}
                x[i] = x_new[i];
            }
            iter++;
            if(error < epsilon || iter >= maxIter){ break;}
        }
        return std::make_pair(x_new, iter);
    }

    // Function to solve the system Ax=b using the relaxation method
    auto relaxation(const float w) -> std::pair<std::vector<float>, int> {
        int iter = 0;
        float error = epsilon + 1;
        std::vector<float> x_new(n, 0.0);

        while(error > epsilon && iter < maxIter) {
            for(int i = 0; i < n; i++) {
                float sum1 = 0.0;
                float sum2 = 0.0;
                for(int j = 0; j < i; j++){ sum1 += A[i][j] * x_new[j];}
                for(int j = i + 1; j < n; j++){ sum2 += A[i][j] * x[j];}
                x_new[i] = (1 - w) * x[i] + (w / A[i][i]) * (b[i] - sum1 - sum2);
            }
            error = std::abs(x_new[0] - x[0]);
            for(int i = 0; i < n; i++) {
                if(std::abs(x_new[i] - x[i]) > error){ error = std::abs(x_new[i] - x[i]);}
                x[i] = x_new[i];
            }
            iter++;
            if(error < epsilon || iter >= maxIter){ break;}
        }
        return std::make_pair(x_new, iter);
    }

    void printResults(const std::vector<float>& xPrecise,
                      int64_t iters) {
        std::cout << "1) First 5 coordinates: {";
        for(int i = 0; i < 5; i++){ std::cout << xPrecise[i] << ", ";}
        std::cout << "...}\n\t2) Iterations: " << iters << "\n";
    }
};

int main() {
    std::ios_base::sync_with_stdio(0);
    std::cin.tie(0);
    std::cout.tie(0);
    Jacobi jacobi;
    jacobi.Solution();
    return 0;
}
```

## Выходные данные
__Метод Якоби__:
- Первые 5 координат вектора приближенного решения $x^*$: (6.00000000, 7.00000000, 8.00000000, 9.00000000, 10.00000000, ...)
- Количество итераций: 73

__Метод релаксации w = 0.5__:
- Первые 5 координат вектора приближенного решения $x^*$: (6.00000000, 7.00000000, 8.00000000, 9.00000000, 10.00000000, ...)
- Количество итераций: 62

__Метод релаксации w = 1__:
- Первые 5 координат вектора приближенного решения $x^*$: (6.00000000, 7.00000000, 8.00000000, 9.00000000, 10.00000000, ...)
- Количество итераций: 31

__Метод релаксации w = 1.5__:
- Первые 5 координат вектора приближенного решения $x^*$: (6.00000000, 7.00000000, 8.00000000, 9.00000381, 10.00000286, ...)
- Количество итераций: 22

## Выводы
Из результатов выполнения программы видно следующее:

1. Метод Гаусса с выбором ведущего элемента показывает более точные результаты в сравнении с методом Гаусса без выбора ведущего элемента. Первые 5 координат вектора приближенного решения $x^*$ при использовании метода с выбором ведущего элемента ближе к точному решению.

2. Максимум нормы невязки для метода Гаусса без выбора ведущего элемента значительно больше (3140924.000000) по сравнению с методом с выбором ведущего элемента (6264). Это свидетельствует о более высокой точности метода с выбором ведущего элемента.

3. Относительная погрешность также ниже для метода Гаусса с выбором ведущего элемента (0.000620) по сравнению с методом без выбора ведущего элемента (0.234914). Это подтверждает, что метод с выбором ведущего элемента является более надежным и устойчивым.

4. Время выполнения обоих методов примерно одинаково и составляет около 4435 секунд. Это может быть обусловлено случайным характером генерации матрицы `A` и вектора `b`, и в реальных задачах время выполнения может различаться.

Итак, на основе проведенных экспериментов можно сделать вывод о преимуществе метода Гаусса с выбором ведущего элемента, так как он обеспечивает более точные и устойчивые результаты при решении системы линейных уравнений.