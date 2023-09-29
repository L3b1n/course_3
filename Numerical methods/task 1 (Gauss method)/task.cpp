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
        double timeElapsedWithoutLeading = solveUsingGaussWithoutLeading(xWithoutLeading);

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
        std::cout << "...)\n\tМаксимум нормы невязки: " << findMaximumNorm(residualWithLeading, 1) << "\n";
        std::cout << "\tОтносительнвя погрешность: " << std::fixed << relErrorWithLeading << "\n";
        std::cout << "\tВремя, затраченное на вычисления: " << std::fixed << timeElapsedWithLeading << '\n';

        std::cout << "Без ведущего: \n\tx*: (";
        for(int i = 0; i < 5; i++){ std::cout << xWithoutLeading[i] << ", ";}
        std::cout << "...)\n\tМаксимум нормы невязки: " << findMaximumNorm(residualWithoutLeading, 1) << "\n";
        std::cout << "\tОтносительная погрешность: " << std::fixed << relErrorNoLeading << "\n";
        std::cout << "\tВремя, затраченное на вычисления: " << std::fixed << timeElapsedWithoutLeading << '\n';
    }

private:
    float *A; // Матрица А
    float *b; // Правая часть уравнения Ax = b
    const int n = 2000; // Порядок матрицы А
    const int numberInGroup = 6; // Номер в списке группы
    float *preciseSolution; // Точное решение Ax = b

    // Используются формулы (6)-(9) на с.3
    // Возвращает время, затраченное на вычисления (millisec)
    template <class T>
    double solveUsingGaussWithoutLeading(T *x)
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

    // Используется формула (6)-(8) с.14
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
            x[i] = (1 / tempA[i * n + i]) * (tempB[i] - rightSum);
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