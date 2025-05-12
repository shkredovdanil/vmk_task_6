#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "functions.h"

typedef double (*Func)(double);

double root(Func f, Func g, double a, double b, double eps, int *iter)
{
    double fa = f(a), fb = f(b);
    double ga = g(a), gb = g(b);

    do
    {
        double c = (a + b) / 2;
        double fc = f(c), gc = g(c);

        if (((fa - ga) * (fc - gc)) <= 0)
        {
            b = c;
            fb = fc;
            gb = gc;
        }
        else
        {
            a = c;
            fa = fc;
            ga = gc;
        }

        (*iter)++;
    } while (fabs(fa - fb) >= eps && (*iter) < 1e6);

    return a;
}

double integral(Func f, double a, double b, double eps)
{
    int n = 2;
    double h = (b - a) / n;
    double prev_integral = 0, cur_integral = (f(a) + 4 * f(a + h) + f(b)) * h / 3;

    while (fabs(prev_integral - cur_integral) >= eps)
    {
        n *= 2;
        h = (b - a) / n;
        prev_integral = cur_integral;
        cur_integral = f(a) + f(b);

        for (int i = 1; i < n; i++)
        {
            double x = a + i * h;

            if (i % 2 == 0)
            {
                cur_integral += 2 * f(x);
            }
            else
            {
                cur_integral += 4 * f(x);
            }
        }

        cur_integral *= h / 3;
    }

    return cur_integral;
}

double g1(double x)
{
    return log(x);
}

double g2(double x)
{
    return -2 * x + 14;
}

double g3(double x)
{
    return 1 / (2 - x) + 6;
}

void helper(void)
{
    printf("Справка:\n"
           "--help                     Показать это сообщение;\n"
           "--print-functions          Вывести функции в соответствии с их нумерацией;\n"
           "--print-roots              Напечатать точки пересечения функций;\n"
           "--print-iters              Напечать количество итераций для нахождения точек пересечения;\n"
           "--print-area               Напечатать площадь фигуры, ограниченной функциями 1-3;\n"
           "--test-integral f a b eps  Тест интеграла от функции f на отрезке [a, b] с точностью eps;\n"
           "--test-root f1 f2 a b eps  Тест пересечения функций f1, f2 на отрезке [a, b] с точностью eps.\n\n"
           "Пример правильного ввода:  ./main --print-roots --print-iters\n"
           "Пример правильного ввода:  ./main --test-root 1 2 1 3 0.001\n"
           "Не спешивайте print и test!!!\n");
}

void print_function(void)
{
    printf("1) y = ln(x)\n"
           "2) y = -2x + 14\n"
           "3) y = 1 / (2 - x) + 6\n");
}

int main(int argc, char *argv[])
{
    if (argc <= 1)
    {
        printf("Не указаны опции для программы. Воспользуйтесь справкой.\n");
        helper();

        return 1;
    }

    int print_roots = 0, print_iters = 0, print_area = 0, test_integ = 0, test_root = 0;
    int f1_root = 0, f2_root = 0, f_integral = 0;
    double a = 0, b = 0, eps1 = 0, eps2 = 0;

    // Обработчик ошибок и опций
    for (int i = 1; i < argc; i++)
    {
        if (!strcmp(argv[i], "--help"))
        {
            helper();

            return 1;
        }

        if (!strcmp(argv[i], "--print-functions"))
        {
            print_function();

            return 1;
        }

        if (!strcmp(argv[i], "--print-area"))
        {
            print_area = 1;
        }

        if (!strcmp(argv[i], "--print-roots"))
        {
            print_roots = 1;
        }

        if (!strcmp(argv[i], "--print-iters"))
        {
            print_iters = 1;
        }

        if (!strcmp(argv[i], "--test-root"))
        {
            test_root = 1;

            if (i + 5 != argc - 1)
            {
                printf("Ошибка. Для --test-root требуется ровно 5 аргументов.\n");
                helper();

                return 1;
            }

            f1_root = atoi(argv[i + 1]);
            f2_root = atoi(argv[i + 2]);

            if (f1_root > 3 || f1_root < 1 || f2_root > 3 || f2_root < 1)
            {
                printf("Ошибка. Функции имеют нумерацию от 1 до 3.\n");
                helper();

                return 1;
            }

            if (f1_root == f2_root)
            {
                printf("Ошибка. Задайте две разные функции\n");
                helper();

                return 1;
            }

            char *ptrEnd;
            a = strtod(argv[i + 3], &ptrEnd);

            if (*ptrEnd != '\0')
            {
                printf("Ошибка. Некорректное значение %s\n", argv[i + 3]);

                return 1;
            }

            b = strtod(argv[i + 4], &ptrEnd);

            if (*ptrEnd != '\0')
            {
                printf("Ошибка. Некорректное значение %s\n", argv[i + 4]);

                return 1;
            }

            if (a >= b)
            {
                printf("Ошибка. Задайте верные пределы нахождения точек пересечения функций. (a < b)\n");

                return 1;
            }

            if ((f1_root == 3 || f2_root == 3) && 2 >= a && 2 <= b)
            {
                printf("Ошибка. Функция %d частично не определена на промежутке [%lf, %lf]."
                       "Задайте иные промежутки.\n",
                       (f1_root == 3 ? f1_root : f2_root), a, b);

                return 1;
            }

            if ((f1_root == 1 || f2_root == 1) && a <= 0)
            {
                printf("Ошибка. Функция %d частично не определена на промежутке [%lf, %lf]."
                       "Задайте иные промежутки.\n",
                       (f1_root == 1 ? f1_root : f2_root), a, b);

                return 1;
            }

            eps1 = strtod(argv[i + 5], &ptrEnd);

            if (*ptrEnd != '\0' || eps1 <= 0)
            {
                printf("Ошибка. Некорректное значение eps.\n");

                return 1;
            }
        }
        if (!strcmp(argv[i], "--test-integral"))
        {
            test_integ = 1;

            if (i + 4 != argc - 1)
            {
                printf("Ошибка. Для --test-integral требуется ровно 4 аргумента.\n");
                helper();

                return 1;
            }

            f_integral = atoi(argv[i + 1]);

            if (f_integral > 3 || f_integral < 1)
            {
                printf("Ошибка. Функции имеют нумерацию от 1 до 3.\n");
                helper();

                return 1;
            }

            char *ptrEnd;
            a = strtod(argv[i + 2], &ptrEnd);

            if (*ptrEnd != '\0')
            {
                printf("Ошибка. Некорректное значение %s\n", argv[i + 2]);

                return 1;
            }

            b = strtod(argv[i + 3], &ptrEnd);

            if (*ptrEnd != '\0')
            {
                printf("Ошибка. Некорректное значение %s\n", argv[i + 3]);

                return 1;
            }

            if (a >= b)
            {
                printf("Ошибка. Задайте верные пределы нахождения точек пересечения функций. (a < b)\n");

                return 1;
            }

            if (f_integral == 3 && 2 >= a && 2 <= b)
            {
                printf("Ошибка. Функция %d частично не определена на промежутке [%lf, %lf]."
                       "Задайте иные промежутки.\n",
                       f_integral, a, b);

                return 1;
            }

            if (f_integral == 1 && a <= 0)
            {
                printf("Ошибка. Функция %d частично не определена на промежутке [%lf, %lf]."
                       "Задайте иные промежутки.\n",
                       (int)f_integral, a, b);

                return 1;
            }

            eps2 = strtod(argv[i + 4], &ptrEnd);

            if (*ptrEnd != '\0' || eps2 <= 0)
            {
                printf("Ошибка. Некорректное значение eps.\n");

                return 1;
            }
        }
    }

    if (test_root)
    {
        double (*functions[])(double) = {f1, f2, f3};
        int iter = 0;

        double x = root(functions[f1_root - 1], functions[f2_root - 1], a, b, eps1, &iter);

        printf("Результат тестирования нахождения точки пересечения двух функций:\n"
               "\tТочка пересечения:\tx = %lf\n"
               "\tКоличество итераций:\t%d\n",
               x, iter);

        return 0;
    }

    if (test_integ)
    {
        double (*functions[])(double) = {f1, f2, f3};

        double x = integral(functions[f_integral - 1], a, b, eps2);

        printf("Результат тестирования нахождения площади между функцией %d и x = 0:\n"
               "\tПлощадь области:\tx = %lf\n",
               f_integral, x);

        return 0;
    }

    int iter1 = 0, iter2 = 0, iter3 = 0;

    double x1 = root(f1, f2, 4, 8, 0.001, &iter1);
    double x2 = root(f1, f3, 2.1, 4, 0.001, &iter2);
    double x3 = root(f2, f3, 4, 6, 0.001, &iter3);

    double area = integral(f3, x2, x3, 0.001) + integral(f2, x3, x1, 0.001) - integral(f1, x2, x1, 0.001);

    if (print_roots)
    {
        printf("Точки пересечения функций:\n"
               "\t y = lnx \t y = -2x + 14 \t\t при x = %lf\n"
               "\t y = lnx \t y = 1 / (2 - x) + 6 \t при x = %lf\n"
               "\t y = -2x + 14 \t y = 1 / (2 - x) + 6 \t при x = %lf\n",
               x1, x2, x3);
    }

    if (print_iters)
    {

        printf("Количество итераций для нахождения точек пересечения функций:\n"
               "\t y = lnx \t y = -2x + 14 \t\t за %d\n"
               "\t y = lnx \t y = 1 / (2 - x) + 6 \t за %d\n"
               "\t y = -2x + 14 \t y = 1 / (2 - x) + 6 \t за %d\n",
               iter1, iter2, iter3);
    }

    if (print_area)
    {
        printf("Площадь фигуры, ограниченной функциями 1-3 равна: %lf\n", area);
    }

    return 0;
}