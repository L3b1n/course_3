# Задание 1. Выпуклые множества

## Группа 3
№5.18 Вариант $((N_1+4) mod 20) + 1$   
№5.22 Вариант $((N_1+5) mod 30) + 1$  
$N_1$ - номер в списке  

## Ссылки
[Условие](ConvexSets.pdf)  
[Решение](task1.pdf)

## 5.18
Доказать, что множество $X=\{x:\ ax_1^2+bx_1x_2+cx_2^2 \leq 0,\ x \geq 0\}$ является выпуклым конусом и изобразить его на плоскости (числа a, b, c заданы в табл. 5.2).  
_Таблица 5.2_  
<table>
    <thead>
        <tr>
            <th align="center">Варианты</th>
            <th align="center">1</th>
            <th align="center">2</th>
            <th align="center">3</th>
            <th align="center">4</th>
            <th align="center">5</th>
            <th align="center">6</th>
            <th align="center">7</th>
            <th align="center">8</th>
            <th align="center">9</th>
            <th align="center">10</th>
        </tr>
    </thead>
    <tbody>
        <tr>
            <td align="center">a</td>
            <td align="center">2</td>
            <td align="center">3</td>
            <td align="center">2</td>
            <td align="center">2</td>
            <td align="center">4</td>
            <td align="center">4</td>
            <td align="center">5</td>
            <td align="center">5</td>
            <td align="center">3</td>
            <td align="center">3</td>
        </tr>
        <tr>
            <td align="center">b</td>
            <td align="center">−7</td>
            <td align="center">4</td>
            <td align="center">−5</td>
            <td align="center">−5</td>
            <td align="center">−15</td>
            <td align="center">−11</td>
            <td align="center">−13</td>
            <td align="center">−17</td>
            <td align="center">−1</td>
            <td align="center">-8</td>
        </tr>
        <tr>
            <td align="center">c</td>
            <td align="center">6</td>
            <td align="center">−4</td>
            <td align="center">2</td>
            <td align="center">−3</td>
            <td align="center">9</td>
            <td align="center">−6</td>
            <td align="center">6</td>
            <td align="center">6</td>
            <td align="center">−4</td>
            <td align="center">4</td>
        </tr>
    </tbody>
    <thead>
        <tr>
            <th align="center">Варианты</th>
            <th align="center">11</th>
            <th align="center">12</th>
            <th align="center">13</th>
            <th align="center">14</th>
            <th align="center">15</th>
            <th align="center">16</th>
            <th align="center">17</th>
            <th align="center">18</th>
            <th align="center">19</th>
            <th align="center">20</th>
        </tr>
    </thead>
    <tbody>
        <tr>
            <td align="center">a</td>
            <td align="center">2</td>
            <td align="center">3</td>
            <td align="center">3</td>
            <td align="center">5</td>
            <td align="center">5</td>
            <td align="center">6</td>
            <td align="center">2</td>
            <td align="center">2</td>
            <td align="center">6</td>
            <td align="center">4</td>
        </tr>
        <tr>
            <td align="center">b</td>
            <td align="center">−1/2</td>
            <td align="center">−3/2</td>
            <td align="center">−1/4</td>
            <td align="center">−2/3</td>
            <td align="center">−2</td>
            <td align="center">−1</td>
            <td align="center">7</td>
            <td align="center">−3</td>
            <td align="center">1</td>
            <td align="center">-5</td>
        </tr>
        <tr>
            <td align="center">c</td>
            <td align="center">-3</td>
            <td align="center">-4</td>
            <td align="center">-2</td>
            <td align="center">-4</td>
            <td align="center">-3</td>
            <td align="center">-3</td>
            <td align="center">3</td>
            <td align="center">-5</td>
            <td align="center">−5</td>
            <td align="center">-6</td>
        </tr>
    </tbody>
</table>

## 5.22
Выписать уравнение гиперплоскости, опорной к множеству $X=\{x:\ x_3 \geq x_1^2 + x_2^2\}$ и отделяющейегоотточки $x^*\ =\ (x_1^*,\ x_2^*,\ x_3^*)$,координаты которой заданы в табл. 5.5.  
_Таблица 5.5_  
<table>
    <thead>
        <tr>
            <th align="center">Варианты</th>
            <th align="center">1</th>
            <th align="center">2</th>
            <th align="center">3</th>
            <th align="center">4</th>
            <th align="center">5</th>
            <th align="center">6</th>
            <th align="center">7</th>
            <th align="center">8</th>
            <th align="center">9</th>
            <th align="center">10</th>
        </tr>
    </thead>
    <tbody>
        <tr>
            <td align="center">x1*</td>
            <td align="center">5/4</td>
            <td align="center">4/3</td>
            <td align="center">5/3</td>
            <td align="center">5/4</td>
            <td align="center">5/3</td>
            <td align="center">3/2</td>
            <td align="center">3/2</td>
            <td align="center">5/4</td>
            <td align="center">10/9</td>
            <td align="center">13/9</td>
        </tr>
        <tr>
            <td align="center">x2*</td>
            <td align="center">5/16</td>
            <td align="center">2/3</td>
            <td align="center">5/9</td>
            <td align="center">15/16</td>
            <td align="center">10/9</td>
            <td align="center">3/2</td>
            <td align="center">3/8</td>
            <td align="center">5/8</td>
            <td align="center">10/27</td>
            <td align="center">26/27</td>
        </tr>
        <tr>
            <td align="center">x3*</td>
            <td align="center">15/16</td>
            <td align="center">13/12</td>
            <td align="center">7/9</td>
            <td align="center">23/16</td>
            <td align="center">10/9</td>
            <td align="center">7/4</td>
            <td align="center">13/16</td>
            <td align="center">9/8</td>
            <td align="center">19/18</td>
            <td align="center">11/9</td>
        </tr>
    </tbody>
    <thead>
        <tr>
            <th align="center">Варианты</th>
            <th align="center">11</th>
            <th align="center">12</th>
            <th align="center">13</th>
            <th align="center">14</th>
            <th align="center">15</th>
            <th align="center">16</th>
            <th align="center">17</th>
            <th align="center">18</th>
            <th align="center">19</th>
            <th align="center">20</th>
        </tr>
    </thead>
    <tbody>
        <tr>
            <td align="center">x1*</td>
            <td align="center">1/2</td>
            <td align="center">1/3</td>
            <td align="center">2</td>
            <td align="center">2</td>
            <td align="center">4</td>
            <td align="center">5/4</td>
            <td align="center">3</td>
            <td align="center">4</td>
            <td align="center">0</td>
            <td align="center">4/5</td>
        </tr>
        <tr>
            <td align="center">x2*</td>
            <td align="center">1/2</td>
            <td align="center">2/3</td>
            <td align="center">1</td>
            <td align="center">1</td>
            <td align="center">3</td>
            <td align="center">1</td>
            <td align="center">1</td>
            <td align="center">0</td>
            <td align="center">3</td>
            <td align="center">1/5</td>
        </tr>
        <tr>
            <td align="center">x3*</td>
            <td align="center">1/2</td>
            <td align="center">3/9</td>
            <td align="center">1</td>
            <td align="center">2</td>
            <td align="center">5</td>
            <td align="center">1</td>
            <td align="center">3</td>
            <td align="center">2</td>
            <td align="center">5</td>
            <td align="center">12/25</td>
        </tr>
    </tbody>
    <thead>
        <tr>
            <th align="center">Варианты</th>
            <th align="center">21</th>
            <th align="center">22</th>
            <th align="center">23</th>
            <th align="center">24</th>
            <th align="center">25</th>
            <th align="center">26</th>
            <th align="center">27</th>
            <th align="center">28</th>
            <th align="center">29</th>
            <th align="center">30</th>
        </tr>
    </thead>
    <tbody>
        <tr>
            <td align="center">x1*</td>
            <td align="center">9/8</td>
            <td align="center">5/4</td>
            <td align="center">9/8</td>
            <td align="center">4/3</td>
            <td align="center">5/3</td>
            <td align="center">11/9</td>
            <td align="center">7/5</td>
            <td align="center">3/2</td>
            <td align="center">4/3</td>
            <td align="center">11/9</td>
        </tr>
        <tr>
            <td align="center">x2*</td>
            <td align="center">27/32</td>
            <td align="center">5/4</td>
            <td align="center">9/32</td>
            <td align="center">4/9</td>
            <td align="center">5/6</td>
            <td align="center">22/27</td>
            <td align="center">14/25</td>
            <td align="center">3/4</td>
            <td align="center">8/9</td>
            <td align="center">11/27</td>
        </tr>
        <tr>
            <td align="center">x3*</td>
            <td align="center">3/2</td>
            <td align="center">15/8</td>
            <td align="center">1</td>
            <td align="center">17/18</td>
            <td align="center">11/12</td>
            <td align="center">4/3</td>
            <td align="center">24/25</td>
            <td align="center">1</td>
            <td align="center">23/18</td>
            <td align="center">1</td>
        </tr>
    </tbody>
</table>