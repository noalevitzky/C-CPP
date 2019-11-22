#include "infi.h" // header file to implement
#include <assert.h>
#include <stddef.h>

/**
 * Computes the numerical integration for the given rationale function
 * at the given numbers range, based on Riemann sums.
 * @param f pointer to function
 * @param a starting point of integration
 * @param b ending point of integration
 * @param n number of partitions
 * @return The integration result.
 */
double integration(RealFunction f, double a, double b, unsigned int n)
{
    assert(f != NULL);
    assert(a < b);
    assert(n > 0);

    const double d_x = (b - a) / n;

    double sum = 0;
    for (unsigned int i = 1; i <= n; i++)
    {
        double s = (a + (i - 1) * d_x);     // starting point of current interval
        double m = (s + (d_x / 2));         // middle point of current interval
        double add = f(m) * d_x;
        sum += add;
    }

    return sum;
}

/**
 * Computes a numerical derivative for the given rationale function
 * at the point, for a given epsilon.
 * @param f pointer to function
 * @param x0 point to differentiate
 * @param h step length
 * @return The differentiate.
 */
double derivative(RealFunction f, double x0, double h)
{
    assert(f != NULL);
    assert(h > 0);

    return ((f(x0 + h) - f(x0 - h)) / (2 * h));
}
