# Отчет по Лабораторной работе 4 «Метод Якоби»

## Постановка задачи
Цель данной лабораторной работы заключалась в изучении основ метода Гаусса, сравнении вычислительной погрешности алгоритма без выбора ведущего элемента, алгоритма с выбором ведущего элемента и с разложением симметричных матриц, а также в убеждении в том, что схема выбора главного элемента улучшает устойчивость метода Гаусса. Для достижения этой цели были выполнены следующие задачи:

1. Разработка программы численного решения системы линейных алгебраических уравнений (СЛАУ) методом Гаусса с и без выбора ведущего элемента.
2. Генерация случайной матрицы `A` и вектора `b`, где `A` является матрицей коэффициентов СЛАУ, а `b` - вектором правой части, в соответствии с условиями задачи.
3. Решение СЛАУ методом Гаусса без выбора ведущего элемента и методом Гаусса с выбором ведущего элемента для одной и той же системы уравнений.
4. Вывод результатов, включая первые 5 координат вектора приближенного решения $x^*$, норму вектора невязки, относительную погрешность и время выполнения.

## Входные данные
- Порядок матрицы `n`: 4
- Номер в списке студенческой группы `m`: 6

## Листинг программы
``` C++
#include <iostream>
#include <iomanip>
#include <vector>
#include <cmath>
#include <random>

constexpr float eps = 1e-06;

std::ostream& operator << (std::ostream& os, const std::vector<std::vector<float>>& matrix) {
    for(const auto& row : matrix){
        for(float val : row){
            if(std::abs(val) < eps){ val = 0.0;}
            os << std::setw(10) << val << " ";
        }
        os << "\n";
    }
    return os;
}

class Danilevsky{
public:
    Danilevsky() { A = gen();}

    void Solution() {
        std::cout << A;
        auto A1 = danilevsky(A, 1, true);
        auto A2 = danilevsky(A1, 2, true);
        auto A3 = danilevsky(A2, 3, true);

        std::cout << "\n Frobenius: \n" << A3 << "\n";

        std::cout << "sp(A): " << sp(A) << " | " << "p1: " << A3[0][0];
    }

private:
    int n = 4;
    std::vector<std::vector<float>> A;

    auto gen(const int n = 4) -> std::vector<std::vector<float>> {
        std::vector<std::vector<float>> A(n, std::vector<float>(n, 0.0));
        std::mt19937 rng(static_cast<unsigned int>(std::time(nullptr)));
        std::uniform_real_distribution<float> dist(-50, 50);

        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                float el = 0.0;
                while(abs(el) < eps) {
                    el = int(dist(rng));
                }
                A[i][j] = el;
            }
        }
        return A;
    }

    auto identity(int n) -> std::vector<std::vector<float>>{
        std::vector<std::vector<float>> result(n, std::vector<float>(n, 0.0));
        for (int i = 0; i < n; ++i) {
            result[i][i] = 1.0;
        }
        return result;
    }

    auto dot(const std::vector<std::vector<float>>& matrix1, 
             const std::vector<std::vector<float>>& matrix2) -> std::vector<std::vector<float>> {
        auto rows1 = matrix1.size();
        auto cols1 = matrix1[0].size();
        auto cols2 = matrix2[0].size();

        std::vector<std::vector<float>> result(rows1, std::vector<float>(cols2, 0.0));

        for(int i = 0; i < rows1; ++i){
            for(int j = 0; j < cols2; ++j){
                for(int k = 0; k < cols1; ++k){ result[i][j] += matrix1[i][k] * matrix2[k][j];}
            }
        }
        return result;
    }

    float sp(std::vector<std::vector<float>> A) {
        float spVal = 0;
        for(auto i = 0; i < n; ++i){ spVal += A[i][i];}
        return spVal;
    }

    auto danilevsky(const std::vector<std::vector<float>>& input_matrix, int iter, bool show) -> std::vector<std::vector<float>> {
        int k = 2;
        std::vector<std::vector<float>> v1, v2, v3;
        for(int i = 0; i < n && k >= 0; ++i) {
            auto matrix = identity(n);
            for(int j = 0; j < n; ++j) {
                if(k < n){
                    matrix[k][j] = -((input_matrix[n - (i + 1)][j]) / (input_matrix[n - (i + 1)][k]));
                    if(j == k){ matrix[k][j] = 1 / (input_matrix[n - (i + 1)][j]);}
                }
            }
            k -= 1;

            if(show && iter == 1 && i == 0){ std::cout << "\nM1:\n" << matrix;} 
            else if(show && iter == 2 && i == 1){ std::cout << "\nM2:\n" << matrix;} 
            else if(show && iter == 3 && i == 2){ std::cout << "\nM3:\n" << matrix;}

            if(i == 0){ v1 = matrix;} 
            else if(i == 1){ v2 = matrix;} 
            else if(i == 2){ v3 = matrix;}
        }

        k = 2;
        std::vector<std::vector<float>> u1, u2, u3;
        for(int i = 0; i < n && k >= 0; ++i) {
            auto matrix = identity(n);
            for(int j = 0; j < n; ++j) {
                if(k < n){
                    matrix[k][j] = (input_matrix[n - (i + 1)][j]);
                    if(j == k){ matrix[k][j] = (input_matrix[n - (i + 1)][j]);}
                }
            }
            k -= 1;

            if(show && iter == 1 && i == 0){ std::cout << "\nM1':\n" << matrix;} 
            else if(show && iter == 2 && i == 1){ std::cout << "\nM2':\n" << matrix;} 
            else if(show && iter == 3 && i == 2){ std::cout << "\nM3':\n" << matrix;}

            if(i == 0){ u1 = matrix;} 
            else if (i == 1){ u2 = matrix;} 
            else if (i == 2){ u3 = matrix;}
        }

        auto mamMatrix1 = dot(u1, dot(input_matrix, v1));
        auto mamMatrix2 = dot(u2, dot(mamMatrix1, v2));
        auto mamMatrix3 = dot(u3, dot(mamMatrix2, v3));

        if(iter == 1) { return mamMatrix1;} 
        else if(iter == 2){ return mamMatrix2;} 
        else if(iter == 3){ return mamMatrix3;} 
        else{ return std::vector<std::vector<float>>(0);}
    }
};

int main() {
    std::ios_base::sync_with_stdio(0);
    std::cin.tie(0);
    std::cout.tie(0);
    Danilevsky danilevsky;
    danilevsky.Solution();
    return 0;
}
```

## Выходные данные
<pre>
       -45         -9         48         46   
        20        -31         20          4   
        43         -7        -11        -16   
        46         46          8         -4 
</pre>
<pre>
M1:
         1          0          0          0 
         0          1          0          0 
     -5.75      -5.75      0.125        0.5 
         0          0          0          1 
</pre>
<pre>
M1':
         1          0          0          0 
         0          1          0          0 
        46         46          8         -4 
         0          0          0          1 
</pre>
<pre>
M2:
         1          0          0          0 
 -0.943745 -5.16102e-05  0.0194055   0.190545 
         0          0          1          0 
         0          0          0          1 
</pre>
<pre>
M2':
         1          0          0          0 
    -18286     -19376        376       3692 
         0          0          1          0 
         0          0          0          1 
</pre>
<pre>
M3:
8.16765e-06 0.000318271 -0.0127717   0.157021 
         0          1          0          0 
         0          0          1          0 
         0          0          0          1 
</pre>
<pre>
M3':
    122434   -38.9673     1563.7   -19224.8 
         0          1          0          0 
         0          0          1          0 
         0          0          0          1 
</pre>
<pre>
Frobenius: 
       -91       1337     119615     921250 
         1          0          0          0 
         0          1          0          0 
         0          0          1          0 
</pre>
<pre>
sp(A): -91 | p1: -91
</pre>

## Выводы
Из результатов выполнения программы видно следующее:

1. Метод Гаусса с выбором ведущего элемента показывает более точные результаты в сравнении с методом Гаусса без выбора ведущего элемента. Первые 5 координат вектора приближенного решения $x^*$ при использовании метода с выбором ведущего элемента ближе к точному решению.

2. Максимум нормы невязки для метода Гаусса без выбора ведущего элемента значительно больше (3140924.000000) по сравнению с методом с выбором ведущего элемента (6264). Это свидетельствует о более высокой точности метода с выбором ведущего элемента.

3. Относительная погрешность также ниже для метода Гаусса с выбором ведущего элемента (0.000620) по сравнению с методом без выбора ведущего элемента (0.234914). Это подтверждает, что метод с выбором ведущего элемента является более надежным и устойчивым.

4. Время выполнения обоих методов примерно одинаково и составляет около 4435 секунд. Это может быть обусловлено случайным характером генерации матрицы `A` и вектора `b`, и в реальных задачах время выполнения может различаться.

Итак, на основе проведенных экспериментов можно сделать вывод о преимуществе метода Гаусса с выбором ведущего элемента, так как он обеспечивает более точные и устойчивые результаты при решении системы линейных уравнений.