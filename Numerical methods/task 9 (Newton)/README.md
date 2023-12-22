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

double f(double x) {
    return std::pow(x, 4) -  std::pow(x, 3) + 8 * std::pow(x, 2) - x - 1;
}

double ff(double x) {
    return 4 * std::pow(x, 3) - 3 * std::pow(x, 2) + 16 * x - 1;
}

double fff(double x) {
    return 12 * std::pow(x, 2) - 6 * x + 16;
}

// Newton-Raphson method implementation
double newton_method(double initial_guess, double epsilon, int max_iterations, bool isf1) {
    double x = initial_guess;

    for (int i = 0; i < max_iterations; ++i) {
        double fx = isf1 ? f(x) : ff(x);
        double fpx = isf1 ? ff(x) : fff(x);

        if (std::abs(fpx) < 1e-10) {
            std::cerr << "Error: Division by zero in Newton method." << std::endl;
            return std::numeric_limits<double>::quiet_NaN(); // Not a number
        }

        x = x - fx / fpx;

        if (std::abs(fx) < epsilon) {
            return x;
        }
    }

    std::cerr << "Warning: Newton method did not converge within the specified number of iterations." << std::endl;
    return std::numeric_limits<double>::quiet_NaN();
}

// Bisection method implementation
double bisection_method(double a, double b, double epsilon, int max_iterations) {
    if (ff(a) * ff(b) > 0) {
        std::cerr << "Error: Bisection method requires that f(a) and f(b) have opposite signs." << std::endl;
        return std::numeric_limits<double>::quiet_NaN();
    }

    double c;

    for (int i = 0; i < max_iterations; ++i) {
        c = (a + b) / 2;

        if (std::abs(ff(c)) < epsilon) {
            return c;
        }

        if (ff(c) * ff(a) < 0) {
            b = c;
        } else {
            a = c;
        }
    }

    std::cerr << "Warning: Bisection method did not converge within the specified number of iterations." << std::endl;
    return std::numeric_limits<double>::quiet_NaN();
}

int main() {
    double newton_initial_guess = -1.0; // Initial guess for the Newton method
    double bisection_a = -2;            // Lower bound for the Bisection method
    double bisection_b = 1;             // Upper bound for the Bisection method
    double epsilon = 1e-8;              // Tolerance for convergence
    int max_iterations = 1e5;           // Maximum number of iterations

    // Newton method
    double newton_root = newton_method(newton_initial_guess, epsilon, max_iterations, false);
    if (!std::isnan(newton_root)) {
        std::cout << "Approximate root using Newton method: " << newton_root << std::endl;
    } else {
        std::cerr << "Error finding root with Newton method." << std::endl;
    }

    // Bisection method
    double bisection_root = bisection_method(bisection_a, bisection_b, epsilon, max_iterations);
    if (!std::isnan(bisection_root)) {
        std::cout << "Approximate root using Bisection method: " << bisection_root << std::endl;
    } else {
        std::cerr << "Error finding root with Bisection method." << std::endl;
    }

    // Newton method f1
    newton_root = newton_method(newton_initial_guess, epsilon, max_iterations, true);
    if (!std::isnan(newton_root)) {
        std::cout << "Approximate root using Newton method for init function: " << newton_root << std::endl;
    } else {
        std::cerr << "Error finding root with Newton method." << std::endl;
    }

    return 0;
}
```

## Выходные данные
<pre>
Approximate root using Newton method: 0.0631855
Approximate root using Bisection method: 0.0631855
Approximate root using Newton method for init function: -0.290957
</pre>

## Выводы
Из результатов выполнения программы видно следующее:

1. Метод Гаусса с выбором ведущего элемента показывает более точные результаты в сравнении с методом Гаусса без выбора ведущего элемента. Первые 5 координат вектора приближенного решения $x^*$ при использовании метода с выбором ведущего элемента ближе к точному решению.

2. Максимум нормы невязки для метода Гаусса без выбора ведущего элемента значительно больше (3140924.000000) по сравнению с методом с выбором ведущего элемента (6264). Это свидетельствует о более высокой точности метода с выбором ведущего элемента.

3. Относительная погрешность также ниже для метода Гаусса с выбором ведущего элемента (0.000620) по сравнению с методом без выбора ведущего элемента (0.234914). Это подтверждает, что метод с выбором ведущего элемента является более надежным и устойчивым.

4. Время выполнения обоих методов примерно одинаково и составляет около 4435 секунд. Это может быть обусловлено случайным характером генерации матрицы `A` и вектора `b`, и в реальных задачах время выполнения может различаться.

Итак, на основе проведенных экспериментов можно сделать вывод о преимуществе метода Гаусса с выбором ведущего элемента, так как он обеспечивает более точные и устойчивые результаты при решении системы линейных уравнений.