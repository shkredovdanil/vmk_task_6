#include <math.h>
#include <stdio.h>
#include <string.h>
#include "functions.h"

typedef double (*Func)(double);

double root(Func f, Func g, double a, double b, double eps)
{
    double fa = f(a), fb = f(b);
    double ga = g(a), gb = g(b);

    int iter = 0;

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

        iter++;
    } while (fabs(fa - fb) >= eps && iter < 1e6);

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
    return ln(x);
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
           "--print-functions          Вывести функции в соответствии с их нумерацией.\n"
           "--print-roots              Напечатать точки пересечения функций;\n"
           "--print-iters              Напечать количество итераций для нахождения точек пересечения;\n"
           "--test-integral f a b eps  Тест интеграла от функции f на отрезке [a, b] с точностью eps;\n"
           "--test-root f1 f2 a b eps  Тест пересечения функций f1, f2 на отрезке [a, b] с точностью eps.\n\n"
           "Пример правильного ввода:  ./main --print-roots --print-iters\n"
           "Пример правильного ввода:  ./main --test-root 1 2 1 3 0.0001\n"
           "Не спешивайте print и test!!!");
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

    int print_roots = 0, print_iters = 0, test_integ = 0, test_root = 0;
    double a = 0, b = 0, eps = 0, f1_root = 0, f2_root = 0;
    double f_integral = 0;

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

        if (!strcmp(argv[i], "--print-roots"))
        {
            print_roots = 1;
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
                printf("Ошибка функции имеют нумерацию от 1 до 3.\n");
                helper();

                return 1;
            }

            if (f1_root == f2_root)
            {
                printf("Ошибка. Задайте две разные функции");
                helper();

                return 1;
            }

            char *ptrEnd;
            a = strtod(argv[i + 3], &ptrEnd);

            if (ptrEnd != '\0')
            {
                printf("Ошибка. Некорректное значение %s", argv[i + 3]);

                return 1;
            }

            b = strtod(argv[i + 4], &ptrEnd);

            if (ptrEnd != '\0')
            {
                printf("Ошибка. Некорректное значение %s", argv[i + 4]);

                return 1;
            }

            if (a >= b)
            {
                printf("Ошибка. Задайте верные пределы нахождения точек пересечения функций. (a < b)");

                return 1;
            }

            if ((f1_root == 1 || f2_root == 1) && a <= 0)
            {
                printf("Ошибка. Функция %d частично не определена на промежутке [%lf, %lf]."
                       "Задайте иные промежутки.",
                       f1_root == 1 ? f1_root : f2_root, b);

                return 1;
            }

            double eps = strtod(argv[i + 5], &ptrEnd);

            if (ptrEnd != '\0' || eps <= 0)
            {
                printf("Ошибка. Некорректное значение eps.\n");

                return 1;
            }
        }
    }

    return 0;
}