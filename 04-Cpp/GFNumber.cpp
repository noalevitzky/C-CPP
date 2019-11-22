#include "GFNumber.h"
#include <cassert>
#include <unordered_set>

//helper functions
/**
 *
 * @param x
 * @return function
 */
long f(long x) { return ((x * x) + 1); }

/**
 *
 * @param n
 * @param order
 * @return modulo of number n and order
 */
long mod(const long n, const long order)
{
    long result = n % order;
    if (result < 0)
    {
        return result + order;
    }
    return result;
}

/** constructor for given number and field */
GFNumber::GFNumber(long n, const GField& field)
{
    _field = field;
    _n = mod(n, getField().getOrder());
}

/** prints out GFNumber's prime factors */
void GFNumber::printFactors()
{
    std::cout << getNumber() << "=";

    int numOfFactors = 0;
    GFNumber* result = getPrimeFactors(&numOfFactors);

    if (numOfFactors == 0 || getIstPrime())
    {
        std::cout << getNumber() << "*1" << std::endl;
        delete[] result;
        return;
    }

    size_t i = 0;
    for (; i < (size_t) numOfFactors - 1; i++)
    {
        std::cout << result[i].getNumber() << "*";
    }
    std::cout << result[i].getNumber() << std::endl;
    delete[] result;
}

/**
  *
  * @param n
  * @return pointer to an array of factors of n
  */
void GFNumber::_trialDivision(GFNumber** result, int* numOfFactors, long n)
{
    long i = 2;
    while (i <= (long) floorl(sqrtl(n)))
    {
        if (n % i != 0 || !getField().createNumber(i).getIstPrime())
        {
            i++;
        }
        else
        {
            _addFactor(result, numOfFactors, i);
            n /= (long) floorl(i);
        }
    }
    if (n > 1)
    {
        _addFactor(result, numOfFactors, n);
    }
}

/**
 *
 * @param n
 * @return factor of number n, or -1 otherwise
 */
long GFNumber::_pollardRho(long n)
{
    long x = (1 + (long) random() % (n - 1));
    long y;
    long p = 1;
    while (p == 1)
    {
        x = f(x);
        y = f(f(x));
        auto temp = getField().createNumber(std::abs(x - y));
        auto num = getField().createNumber(n);
        p = getField().gcd(temp, num).getNumber();
    }
    return p == n ? -1 : p;
}

/**
 * add a single factor
 * @param n
 */
void GFNumber::_addFactor(GFNumber** result, int* numOfFactors, long n)
{
    if (*result == nullptr)
    {
        *result = new GFNumber[1];
        *result[*numOfFactors] = getField().createNumber(n);
        *numOfFactors += 1;
        return;
    }

    auto* temp = new GFNumber[*numOfFactors + 1];

    //copy
    for (size_t i = 0; i < (size_t) *numOfFactors; i++)
    {
        temp[i] = (*result)[i];
    }
    delete[] *result;
    temp[*numOfFactors] = getField().createNumber(n);
    *numOfFactors += 1;
    *result = temp;
}

/**
 *
 * @param n
 * @return pointer to an array of factors of n
 */
GFNumber* GFNumber::getPrimeFactors(int* numOfFactors)
{
    long num = getNumber();
    GFNumber* result = nullptr;

    if (num == 0 || num == 1 || getIstPrime())
    {
        return result;
    }

    while (num % 2 == 0)
    {
        _addFactor(&result, numOfFactors, 2);
        num /= 2;
    }

    if (num == 1)
    {
        return result;
    }

    long f = _pollardRho(num);
    while (f != -1)
    {
        _addFactor(&result, numOfFactors, f);
        num /= f;
        f = _pollardRho(num);
    }

    if (num == 1)
    {
        return result;
    }

    //get rest of factors
    _trialDivision(&result, numOfFactors, num);
    return result;
}

GFNumber GFNumber::operator+(const GFNumber& other)
{
    assert(getField() == other.getField());
    return GFNumber(mod(getNumber() + other.getNumber(), getField().getOrder()), getField());
}

GFNumber GFNumber::operator+(const long k)
{
    return GFNumber(mod(getNumber() + k, getField().getOrder()), getField());
}

GFNumber& GFNumber::operator+=(const GFNumber& other)
{
    assert(getField() == other.getField());
    _n = mod(getNumber() + other.getNumber(), getField().getOrder());
    return *this;
}

GFNumber& GFNumber::operator+=(const long k)
{
    _n = mod(getNumber() + k, getField().getOrder());
    return *this;
}

GFNumber GFNumber::operator-(const GFNumber& other)
{
    assert(getField() == other.getField());
    return GFNumber(mod(getNumber() - other.getNumber(), getField().getOrder()), getField());
}

GFNumber GFNumber::operator-(const long k)
{
    return GFNumber(mod(getNumber() - k, getField().getOrder()), getField());
}

GFNumber& GFNumber::operator-=(const GFNumber& other)
{
    assert(getField() == other.getField());
    _n = mod(getNumber() - other.getNumber(), getField().getOrder());
    return *this;
}

GFNumber& GFNumber::operator-=(const long k)
{
    _n = mod(getNumber() - k, getField().getOrder());
    return *this;
}

GFNumber GFNumber::operator*(const GFNumber& other)
{
    assert(getField() == other.getField());
    return GFNumber(mod(getNumber() * other.getNumber(), getField().getOrder()), getField());
}

GFNumber GFNumber::operator*(const long k)
{
    return GFNumber(mod(getNumber() * k, getField().getOrder()), getField());
}

GFNumber& GFNumber::operator*=(const GFNumber& other)
{
    assert(getField() == other.getField());
    _n = mod(getNumber() * other.getNumber(), getField().getOrder());
    return *this;
}

GFNumber& GFNumber::operator*=(const long k)
{
    _n = mod(getNumber() * k, getField().getOrder());
    return *this;
}

GFNumber GFNumber::operator%(const GFNumber& other)
{
    assert(getField() == other.getField());
    return GFNumber(mod(getNumber() % other.getNumber(), getField().getOrder()), getField());
}

GFNumber GFNumber::operator%(long k)
{
    long temp = mod(k, getField().getOrder());
    return GFNumber(mod(getNumber() % temp, getField().getOrder()), getField());
}

GFNumber& GFNumber::operator%=(const GFNumber& other)
{
    assert(getField() == other.getField());
    _n = mod(getNumber() % other.getNumber(), getField().getOrder());
    return *this;
}

GFNumber& GFNumber::operator%=(long k)
{
    long temp = mod(k, getField().getOrder());
    _n = mod(getNumber() % temp, getField().getOrder());
    return *this;
}

bool GFNumber::operator==(const GFNumber& other)
{
    assert(this->getField() == other.getField());
    return this->getNumber() == other.getNumber();
}

bool GFNumber::operator!=(const GFNumber& other)
{
    assert(this->getField() == other.getField());
    return this->getNumber() != other.getNumber();
}

bool GFNumber::operator<(const GFNumber& other)
{
    assert(this->getField() == other.getField());
    return this->getNumber() < other.getNumber();
}

bool GFNumber::operator<=(const GFNumber& other)
{
    assert(this->getField() == other.getField());
    return this->getNumber() <= other.getNumber();
}

bool GFNumber::operator>(const GFNumber& other)
{
    assert(this->getField() == other.getField());
    return this->getNumber() > other.getNumber();
}

bool GFNumber::operator>=(const GFNumber& other)
{
    assert(this->getField() == other.getField());
    return this->getNumber() >= other.getNumber();
}

std::istream& operator>>(std::istream& in, GFNumber& number)
{
    long n;
    GField field;
    std::cin >> n >> field;
    assert(!std::cin.fail());
    number = GFNumber(n, field);
    return in;
}

std::ostream& operator<<(std::ostream& out, const GFNumber& number)
{ return (std::cout << number.getNumber() << " " << number.getField()); }



