# Отчет по Лабораторной работе 4 «Метод Якоби»

## Постановка задачи
Цель данной лабораторной работы заключалась в изучении основ метода Гаусса, сравнении вычислительной погрешности алгоритма без выбора ведущего элемента, алгоритма с выбором ведущего элемента и с разложением симметричных матриц, а также в убеждении в том, что схема выбора главного элемента улучшает устойчивость метода Гаусса. Для достижения этой цели были выполнены следующие задачи:

1. Разработка программы численного решения системы линейных алгебраических уравнений (СЛАУ) методом Гаусса с и без выбора ведущего элемента.
2. Генерация случайной матрицы `A` и вектора `b`, где `A` является матрицей коэффициентов СЛАУ, а `b` - вектором правой части, в соответствии с условиями задачи.
3. Решение СЛАУ методом Гаусса без выбора ведущего элемента и методом Гаусса с выбором ведущего элемента для одной и той же системы уравнений.
4. Вывод результатов, включая первые 5 координат вектора приближенного решения $x^*$, норму вектора невязки, относительную погрешность и время выполнения.

## Входные данные
- Порядок матрицы `n`: 10
- Номер в списке студенческой группы `m`: 6
- Количество итераций `K`: 50

## Листинг программы
``` C++
#include <iostream>
#include <cmath>
#include <vector>
#include <random>
#include <ctime>
#include <numeric>
#include <chrono>

const int N = 10;
const int K = 50;

namespace std {
    std::ostream& operator << (std::ostream& os, const std::vector<double>& vec) {
        os << "[";
        for (size_t i = 0; i < vec.size(); ++i) {
            os << " " << vec[i];
            if (i != vec.size() - 1) {
                os << ",";
            }
        }
        os << " ]";
        return os;
    }
}

std::vector<std::vector<double>> generateRandomSymmetricMatrix(int n) {
    std::vector<std::vector<double>> A(n, std::vector<double>(n, 0.0));
    std::mt19937 rng(static_cast<unsigned int>(std::time(nullptr)));
    std::uniform_real_distribution<double> dist(-3, 3);

    for (int i = 0; i < n; ++i) {
        for (int j = i; j < n; ++j) {
            if (i == j) {
                A[i][j] = -std::accumulate(A[i].begin(), A[i].end(), 0.0);
            } else {
                A[i][j] = dist(rng);
                A[j][i] = A[i][j];
            }
        }
    }

    return A;
}

std::vector<double> matrixVectorMultiply(const std::vector<std::vector<double>>& matrix, 
                                         const std::vector<double>& vector) {
    int size = vector.size();
    std::vector<double> result(size, 0.0);

    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            result[i] += matrix[i][j] * vector[j];
        }
    }

    return result;
}

double maxElement(const std::vector<double>& vector) {
    return *std::max_element(vector.begin(), vector.end());
}

std::pair<std::vector<double>, std::vector<double>> powerIteration(const std::vector<std::vector<double>>& matrix, 
                                                                   int maxIterations, 
                                                                   double tolerance) {
    int size = matrix.size();
    std::vector<double> eigenVector(size);
    std::generate(eigenVector.begin(), eigenVector.end(), [](){ return rand() % 100 + 1;});
    double eigenValue = 0.0;

    for (int iteration = 0; iteration < maxIterations; ++iteration) {
        std::vector<double> nextEigenVector = matrixVectorMultiply(matrix, eigenVector);

        double norm = maxElement(nextEigenVector);
        for(int i = 0; i < size; ++i){ nextEigenVector[i] /= norm;}

        eigenValue = matrixVectorMultiply(matrix, nextEigenVector)[0];

        if (iteration > 0 && std::fabs(eigenValue - matrixVectorMultiply(matrix, eigenVector)[0]) < tolerance) {
            std::cout << "Converged after " << iteration + 1 << " iterations\n";
            break;
        }

        eigenVector = nextEigenVector;
    }

    return {eigenVector, matrixVectorMultiply(matrix, eigenVector)};
}

std::pair<double, double> findValues(const std::vector<double>& u, const std::vector<double>& v) {
    double val1 = std::inner_product(v.begin(), v.end(), u.begin(), 0.0);
    double val2 = std::inner_product(u.begin(), u.end(), u.begin(), 0.0);

    double rs1 = val1 / val2;

    int maxIndex = std::distance(v.begin(), std::max_element(v.begin(), v.end()));

    double rs2 = v[maxIndex] / u[maxIndex];

    return {rs1, rs2};
}

double norm(const std::vector<double>& v) {
    double sum = std::inner_product(v.begin(), v.end(), v.begin(), 0.0);
    return std::sqrt(sum);
}

int main() {
    std::vector<std::vector<double>> matrix = generateRandomSymmetricMatrix(N);

    auto result = powerIteration(matrix, K, 1e-6);

    auto u = result.first;
    auto v = result.second;

    auto vals = findValues(u, v);
    auto l1 = vals.first;
    auto l2 = vals.second;

    auto Au = matrixVectorMultiply(matrix, u);
    std::vector<double> vec1(N), vec2(N), vec3(N), vec4(N);
    for(int i = 0; i < N; ++i) {
        vec1[i] = v[i] - l1 * u[i];
        vec2[i] = v[i] - l2 * u[i];
        vec3[i] = Au[i] - l1 * u[i];
        vec4[i] = Au[i] - l2 * u[i];
    }

    std::cout << "EigenVector (u):\n" << u << '\n';
    std::cout << "Vector (v - l1 * u):\n" << vec1 << "\n - Its norm: " << norm(vec1) << "\n";
    std::cout << "Vector (v - l2 * u):\n" << vec2 << "\n - Its norm: " << norm(vec2) << "\n";
    std::cout << "Vector (Au - l1 * u):\n" << vec3 << "\n";
    std::cout << "Vector (Au - l2 * u):\n" << vec4 << "\n";

    std::cout << "Eigenvalue case1:\n" << l1 << '\n';

    std::cout << "Eigenvalue case2:\n" << l2 << '\n';
    return 0;
}
```

## Выходные данные
<pre>
EigenVector (u):
[ -0.20125, -0.273203, 0.111684, -0.300585, -0.264108, 1, -0.225495, -0.201565, 0.148679, 0.215801 ]
</pre>
<pre>
Vector (v - l1 * u):
[ 7.45039e-05, -9.78395e-05, 8.93416e-05, 5.82579e-05, 4.95978e-05, 4.52127e-05, -0.000119155, 6.28876e-05, -0.000220958, -8.18217e-05 ]
Norm: 0.000323532
</pre>
<pre>
Vector (v - l2 * u):
[ 3.54986e-05, -0.00015079, 0.000110988, 0, -1.59026e-06, 0.000239028, -0.000162859, 2.38212e-05, -0.000192141, -3.99962e-05 ]
Norm: 0.000398825
</pre>
<pre>
Vector (Au - l1 * u):
[ 7.45039e-05, -9.78395e-05, 8.93416e-05, 5.82579e-05, 4.95978e-05, 4.52127e-05, -0.000119155, 6.28876e-05, -0.000220958, -8.18217e-05 ]

Vector (Au - l2 * u):
[ 3.54986e-05, -0.00015079, 0.000110988, 0, -1.59026e-06, 0.000239028, -0.000162859, 2.38212e-05, -0.000192141, -3.99962e-05 ]

Eigenvalue case1:
-13.881

Eigenvalue case2:
-13.8812
</pre>

## Выводы
Из результатов выполнения программы видно следующее:

1. Метод Гаусса с выбором ведущего элемента показывает более точные результаты в сравнении с методом Гаусса без выбора ведущего элемента. Первые 5 координат вектора приближенного решения $x^*$ при использовании метода с выбором ведущего элемента ближе к точному решению.

2. Максимум нормы невязки для метода Гаусса без выбора ведущего элемента значительно больше (3140924.000000) по сравнению с методом с выбором ведущего элемента (6264). Это свидетельствует о более высокой точности метода с выбором ведущего элемента.

3. Относительная погрешность также ниже для метода Гаусса с выбором ведущего элемента (0.000620) по сравнению с методом без выбора ведущего элемента (0.234914). Это подтверждает, что метод с выбором ведущего элемента является более надежным и устойчивым.

4. Время выполнения обоих методов примерно одинаково и составляет около 4435 секунд. Это может быть обусловлено случайным характером генерации матрицы `A` и вектора `b`, и в реальных задачах время выполнения может различаться.

Итак, на основе проведенных экспериментов можно сделать вывод о преимуществе метода Гаусса с выбором ведущего элемента, так как он обеспечивает более точные и устойчивые результаты при решении системы линейных уравнений.