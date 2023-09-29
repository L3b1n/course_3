# Отчет по Лабораторной работе 1 «Метод Гаусса»

## Ссылки на документацию и условие лабораторной
 * [Документация](GaussMethodDescription.pdf)
 * [Условие лабораторной](GaussMethod.pdf)

## Постановка задачи
Цель данной лабораторной работы заключалась в изучении основ метода Гаусса, сравнении вычислительной погрешности алгоритма без выбора ведущего элемента и алгоритма с выбором ведущего элемента, а также в убеждении в том, что схема выбора главного элемента улучшает устойчивость метода Гаусса. Для достижения этой цели были выполнены следующие задачи:

1. Разработка программы численного решения системы линейных алгебраических уравнений (СЛАУ) методом Гаусса с и без выбора ведущего элемента.
2. Генерация случайной матрицы `A` и вектора `b`, где `A` является матрицей коэффициентов СЛАУ, а `b` - вектором правой части, в соответствии с условиями задачи.
3. Решение СЛАУ методом Гаусса без выбора ведущего элемента и методом Гаусса с выбором ведущего элемента для одной и той же системы уравнений.
4. Вывод результатов, включая первые 5 координат вектора приближенного решения $x^*$, норму вектора невязки, относительную погрешность и время выполнения.

## Входные данные
- Порядок матрицы `n`: 2000
- Номер в списке студенческой группы `m`: 6

## Листинг программы
``` C++
#include <chrono>
#include <iomanip>
#include <iostream>
#include "stdlib.h"

class GaussMethod
{
public:
    // Заполняет исходные матрицы
    GaussMethod()
    {
        A = new float[n * n];
        for(int i = 0; i < n; i++)
        {
            for(int j = 0; j < n; j++){ A[i * n + j] = static_cast<float>(std::rand() % 2001 - 1000);} // Генерируем случайное число от -1000  до 1000
        }

        preciseSolution = new float[n];
        for(int i = numberInGroup; i < n + numberInGroup; i++){ preciseSolution[i - numberInGroup] = i;} // Заполняем точное решение

        b = new float[n];
        multiplyMatrices(A, n, n, preciseSolution, n, 1, b);
    }

    void Solution()
    {
        // Решения, получившиеся после применения метода Гаусса
        float *xWithLeading = new float[n];
        float *xWithoutLeading = new float[n];

        double timeElapsedWithLeading = solveUsingGaussWithLeading(xWithLeading);
        double timeElapsedNoLeading = solveUsingGaussNoLeading(xWithoutLeading);

        // Невязки
        float *residualWithLeading = new float[n];
        float *residualWithoutLeading = new float[n];

        findResidual(xWithLeading, residualWithLeading);
        findResidual(xWithoutLeading, residualWithoutLeading);

        float *diffWithLeading = new float[n];
        subtractMatrix(preciseSolution, xWithLeading, diffWithLeading, 1);
        // Относительная погрешность с ведущим
        float relErrorWithLeading = findMaximumNorm(diffWithLeading, 1) / findMaximumNorm(preciseSolution, 1);

        float *diffWithoutLeading = new float[n];
        subtractMatrix(preciseSolution, xWithoutLeading, diffWithoutLeading, 1);
        // Относительная погрешность без ведущего
        float relErrorNoLeading = findMaximumNorm(diffWithoutLeading, 1) / findMaximumNorm(preciseSolution, 1);

        std::cout << "С ведущим: \n\tx*: (";
        for(int i = 0; i < 5; i++){ std::cout << xWithLeading[i] << ", ";}
        std::cout << "...)\n\tМаксимум норма невязки: " << findMaximumNorm(residualWithLeading, 1) << "\n";
        std::cout << "\tОтн. погрешность: " << std::fixed << relErrorWithLeading << "\n";
        std::cout << "\tВремя, затраченное на вычисления: " << std::fixed << timeElapsedWithLeading << '\n';

        std::cout << "Без ведущего: \n\tx*: (";
        for(int i = 0; i < 5; i++){ std::cout << xWithoutLeading[i] << ", ";}
        std::cout << "...)\n\tМаксимум норма невязки: " << findMaximumNorm(residualWithoutLeading, 1) << "\n";
        std::cout << "\tОтн. погрешность: " << std::fixed << relErrorNoLeading << "\n";
        std::cout << "\tВремя, затраченное на вычисления: " << std::fixed << timeElapsedNoLeading << '\n';
    }

private:
    float *A; // Матрица А
    float *b; // Правая часть уравнения Ax = b
    const int n = 2000; // 1500; // Порядок матрицы А
    const int numberInGroup = 6; // Номер в списке группы
    float *preciseSolution; // Точное решение Ax = b

    // Используются формулы (6)-(9) на с.3
    // Возвращает время, затраченное на вычисления (millisec)
    template <class T>
    double solveUsingGaussNoLeading(T *x)
    {
        T *tempA = new T[n * n];
        for(int i = 0; i < n * n; i++){ tempA[i] = A[i];}

        T *tempB = new T[n];
        for(int i = 0; i < n; i++){ tempB[i] = b[i];}

        std::chrono::steady_clock::time_point start = std::chrono::high_resolution_clock::now();
        // Прямой ход без выбора ведущего
        for(int k = 0; k < n - 1; k++)
        {
            for(int i = k + 1; i < n; i++)
            {
                // Находим l_ik = a_ik / a_kk
                T l = tempA[i * n + k] / tempA[k * n + k];

                tempB[i] -= l * tempB[k];
                for(int j = k + 1; j < n; j++){ tempA[i * n + j] -= l * tempA[k * n + j];}
            }
        }

        // Обратный ход
        goInReverseOrder(tempA, tempB, x);
        std::chrono::steady_clock::time_point end = std::chrono::high_resolution_clock::now();

        delete[] tempA; delete[] tempB;
        return std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    }

    // Возвращает время, затраченное на вычисления (millisec)
    template <class T>
    double solveUsingGaussWithLeading(T *x)
    {
        T *tempA = new T[n * n];
        for(int i = 0; i < n * n; i++){ tempA[i] = A[i];}

        T *tempB = new T[n];
        for(int i = 0; i < n; i++){ tempB[i] = b[i];}

        std::chrono::steady_clock::time_point start = std::chrono::high_resolution_clock::now();

        // Прямой ход с выбором ведущего, используя формулы на с.14
        for(int k = 0; k < n - 1; k++)
        {
            int rowOfLeading = findRowOfLeadingByCol(tempA, k); // Находим ведущий
            for(int i = 0; i < n; i++){ std::swap(tempA[rowOfLeading * n + i], tempA[k * n + i]);} // Перемещаем строку с ведущим на нужную позицию в матрице
            std::swap(tempB[rowOfLeading], tempB[k]);

            for(int i = k + 1; i < n; i++)
            {
                T l = tempA[i * n + k] / tempA[k * n + k]; // Находим l_ik = a_ik / a_kk

                tempB[i] -= l * tempB[k];
                for(int j = k + 1; j < n; j++){ tempA[i * n + j] -= l * tempA[k * n + j];}
            }
        }

        // Обратный ход
        goInReverseOrder(tempA, tempB, x);
        std::chrono::steady_clock::time_point end = std::chrono::high_resolution_clock::now();

        delete[] tempA; delete[] tempB;
        return std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    }

    // Находит невязку
    template <class T>
    void findResidual(T *x, T *residual)
    {
        multiplyMatrices(A, n, n, x, n, 1, residual);

        for(int i = 0; i < n; i++){ residual[i] = b[i] - residual[i];}
    }

    template <class T>
    T findMaximumNorm(T *matrix, int m)
    {
        T norm;
        bool isFirstIterationFinished = false;

        for(int j = 0; j < n; j++)
        {
            T sum = std::abs(matrix[j * m + 0]);
            for(int i = 1; i < m; i++){ sum += std::abs(matrix[j * m + i]);}

            if(!isFirstIterationFinished)
            {
                isFirstIterationFinished = true;
                norm = sum;
                continue;
            }

            if(sum > norm){ norm = sum;}
        }
        return norm;
    }

    // Обратный ход, используется формула (9) на с.3
    template <class T>
    void goInReverseOrder(T *tempA, T *tempB, T *x)
    {
        x[n - 1] = tempB[n - 1] / tempA[n * n - 1];
        for(int i = n - 2; i >= 0; i--)
        {
            int j = i + 1;
            T rightSum = tempA[i * n + j] * x[j];
            for(++j; j < n; j++){ rightSum += tempA[i * n + j] * x[j];}
            x[i] = 1 / tempA[i * n + i] * (tempB[i] - rightSum);
        }
    }

    // Находит номер строки ведущего по столбцу
    template <class T>
    int findRowOfLeadingByCol(T *A, int colNum)
    {
        int row = colNum;
        int leading = A[colNum * n + colNum];
        for(int i = colNum + 1; i < n; i++)
        {
            T nextElem = A[i * n + colNum];
            if(std::abs(nextElem) > std::abs(leading))
            {
                leading = nextElem;
                row = i;
            }
        }

        return row;
    }

    // Перемножение матриц
    template <class T>
    void multiplyMatrices(T *first, int firstRows, int firstCols, T *second, int secondRows, int secondCols, T *result)
    {
        for(int i = 0; i < firstRows; i++)
        {
            for(int k = 0; k < secondCols; k++)
            {
                int j = 0;
                T elem = first[i * firstCols + j] * second[j * secondCols + k];
                for(++j; j < firstCols; j++){ elem += first[i * firstCols + j] * second[j * secondCols + k];}
                result[i * secondCols + k] = elem;
            }
        }
    }

    // Вычитание матриц
    template <class T>
    void subtractMatrix(T *subtractFrom, T *toSubtract, T *result, int m)
    {
        for(int i = 0; i < n; i++)
        {
            for(int j = 0; j < m; j++)
            {
                int idx = i * m + j;
                result[idx] = subtractFrom[idx] - toSubtract[idx];
            }
        }
    }
};

int main()
{
    std::ios_base::sync_with_stdio(0);
    std::cin.tie(0);
    std::cout.tie(0);
    GaussMethod gaussMethod;
    gaussMethod.Solution();
    return 0;
}
```

## Выходные данные
__С ведущим элементом__:
- Первые 5 координат вектора приближенного решения $x^*$: (4.90891, 6.3934, 7.1097, 7.83099, 8.38487, ...)
- Максимум нормы невязки: 6264
- Относительная погрешность: 0.000620
- Время, затраченное на вычисления: 4435.000000 секунд

__Без ведущего элемента__:
- Первые 5 координат вектора приближенного решения $x^*$: (77.522385, 264.995941, 139.443802, -150.114716, 82.044792, ...)
- Максимум нормы невязки: 3140924.000000
- Относительная погрешность: 0.234914
- Время, затраченное на вычисления: 4407.000000 секунд

## Выводы
Из результатов выполнения программы видно следующее:

1. Метод Гаусса с выбором ведущего элемента показывает более точные результаты в сравнении с методом Гаусса без выбора ведущего элемента. Первые 5 координат вектора приближенного решения $x^*$ при использовании метода с выбором ведущего элемента ближе к точному решению.

2. Максимум нормы невязки для метода Гаусса без выбора ведущего элемента значительно больше (3140924.000000) по сравнению с методом с выбором ведущего элемента (6264). Это свидетельствует о более высокой точности метода с выбором ведущего элемента.

3. Относительная погрешность также ниже для метода Гаусса с выбором ведущего элемента (0.000620) по сравнению с методом без выбора ведущего элемента (0.234914). Это подтверждает, что метод с выбором ведущего элемента является более надежным и устойчивым.

4. Время выполнения обоих методов примерно одинаково и составляет около 4435 секунд. Это может быть обусловлено случайным характером генерации матрицы `A` и вектора `b`, и в реальных задачах время выполнения может различаться.

Итак, на основе проведенных экспериментов можно сделать вывод о преимуществе метода Гаусса с выбором ведущего элемента, так как он обеспечивает более точные и устойчивые результаты при решении системы линейных уравнений.