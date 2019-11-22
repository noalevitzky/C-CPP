#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include "infi.c" // todo is it?

#define NUM_ARGS 7

/**
 *
 * @param x param
 * @return f1, as stated in the ex description
 */
double f1(double x)
{ return (pow(x, 2) / 4); }

/**
 * note- x division
 * @param x param
 * @return f2, as stated in the ex description
 */
double f2(double x)
{ return ((-1 * pow(x, 3) + 3 * pow(x, 2) + x - 4 * sqrt(x)) / (2 * x * sqrt(x))); }

/**
 *
 * @param x param
 * @return f3, as stated in the ex description
 */
double f3(double x)
{ return (pow(sin(x), 2) - pow(cos(x), 2)); }

/**
 * note- x division
 * @param x param
 * @return f4, as stated in the ex description
 */
double f4(double x)
{ return (sin(x) / (1 + cos(2 * x))); }

/**
 *
 * @param x param
 * @return f5, as stated in the ex description
 */
double f5(double x)
{ return (pow(M_E, x)); }

/**
 * note- x division
 * @param x param
 * @return f6, as stated in the ex description
 */
double f6(double x)
{ return (sinh(2 * x) / (M_E * pow(x, 2))); }

/**
 *
 * @param fStr str representation of function num
 * @param isErr pointer to error indicator
 * @return num of function upon success, failure otherwise
 */
int validateF(const char *fStr, int *isErr)
{
    // validate num is between 1-6
    if (fStr[0] < '1' || fStr[0] > '6' || strlen(fStr) != 1)
    {
        *isErr = EXIT_FAILURE;
        return EXIT_FAILURE;
    }

    // convert to int
    return atoi(fStr);
}

/**
 *
 * @param dStr str representation of double argument
 * @param isErr pointer to error indicator
 * @return num of function upon success, failure otherwise
 */
double validateD(const char *dStr, int *isErr)
{
    if (dStr[0] == ' ' || (strlen(dStr) != 1 && dStr[0] == 0))
    {
        *isErr = EXIT_FAILURE;
        return EXIT_FAILURE;
    }

    char *err;
    double d = strtod(dStr, &err);
    if (*err != 0)
    {
        *isErr = EXIT_FAILURE;
        return EXIT_FAILURE;
    }
    return d;
}

/**
 *
 * @param uStr str representation of unsigned int argument
 * @param isErr pointer to error indicator
 * @return num of function upon success, failure otherwise
 */
unsigned int validateU(const char *uStr, int *isErr)
{
    for (size_t i = 0; i < strlen(uStr); i++)
    {
        if (uStr[i] < '0' || uStr[i] > '9')
        {
            *isErr = EXIT_FAILURE;
            return EXIT_FAILURE;
        }
    }

    // convert to unsigned int
    return atoi(uStr);
}

/**
 * program that uses infi library
 * @param argc
 * @param argv
 * @return if the process was successful. print out calculation results
 */
int main(int argc, char *argv[])
{
    if (argc != NUM_ARGS)
    {
        fprintf(stderr, "Usage: NumericalAnalyzer <function number> <a> <b> <N> <x0> <h>\n");
        return EXIT_FAILURE;
    }

    int isErr = 0;

    // process arguments
    int f = validateF(argv[1], &isErr);
    double a = validateD(argv[2], &isErr);
    double b = validateD(argv[3], &isErr);
    unsigned int n = validateU(argv[4], &isErr);
    double x0 = validateD(argv[5], &isErr);
    double h = validateD(argv[6], &isErr);

    if (isErr == EXIT_FAILURE)
    {
        fprintf(stderr, "Invalid input\n");
        return EXIT_FAILURE;
    }

    // calc integration and derivative
    double integ = 0;
    double deriv = 0;

    switch (f)
    {
        case 1:
            integ =  integration(f1, a, b, n);
            deriv =  derivative(f1, x0, h);
            break;
        case 2:
            integ =  integration(f2, a, b, n);
            deriv =  derivative(f2, x0, h);
            if (x0 == 0)
            {
                fprintf(stderr, "Invalid input\n");
                return EXIT_FAILURE;
            }
            break;
        case 3:
            integ =  integration(f3, a, b, n);
            deriv =  derivative(f3, x0, h);
            break;
        case 4:
            integ =  integration(f4, a, b, n);
            deriv =  derivative(f4, x0, h);
            if (x0 == 0)
            {
                fprintf(stderr, "Invalid input\n");
                return EXIT_FAILURE;
            }
            break;
        case 5:
            integ =  integration(f5, a, b, n);
            deriv =  derivative(f5, x0, h);
            break;
        case 6:
            integ =  integration(f6, a, b, n);
            deriv =  derivative(f6, x0, h);
            if (x0 == 0)
            {
                fprintf(stderr, "Invalid input\n");
                return EXIT_FAILURE;
            }
            break;
        default:
            return EXIT_FAILURE;
    }
    printf("Integral: %0.5f\nDerivative %0.5f", integ, deriv);
    return EXIT_SUCCESS;
}