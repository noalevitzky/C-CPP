#include <iostream>
#include <cassert>
#include <cmath>
#include "GField.h"
#include "GFNumber.h"

/**
 * constructor
 * @param p field char
 * @param l field degree
 */
GField::GField(long p, long l)
{
    long temp = (p < 0)? -p : p;
    assert(GField::isPrime(temp));
    assert(l > 0);
    this->_p = temp;
    this->_l = l;
}

/**
 *
 * @param p number to verify
 * @return true if given p is a prime number, false otherwise
 */
bool GField::isPrime(const long& p)
{
    if (p < 2)
    {
        return false;
    }
    for (int i = 2; i <= sqrtl(p); i++)
    {
        if (p % i == 0)
        {
            return false;
        }
    }
    return true;
}

/**
 *
 * @param a
 * @param b
 * @return gcd of 2 given numbers, based on euclidean algorithm
 */
GFNumber GField::gcd(GFNumber& a, GFNumber& b)
{
    assert(a.getField() == b.getField());
    if (b.getNumber() == 0)
    {
        return a;
    }
    GFNumber temp = a % b;
    return gcd(b, temp);
}

/**
 *
 * @param k long to be converted
 * @return new GFNumber of given number in current field
 */
GFNumber GField::createNumber(const long k)
{ return GFNumber(k, *this); }

/**
 *
 * @param other
 * @return true if current GField is equal to other GField, true otherwise
 */
bool GField::operator==(const GField& other)
{ return (this->getChar() == other.getChar() && this->getDegree() == other.getDegree()); }

/**
 *
 * @param other
 * @return true if current GField is unequal to other GField, true otherwise
 */
bool GField::operator!=(const GField& other)
{ return (this->getChar() != other.getChar() || this->getDegree() != other.getDegree()); }

/**
 *
 * @param in
 * @param field
 * @return reference to input stream, after assigning field values
 */
std::istream& operator>>(std::istream& in, GField& field)
{
    long p, l;
    std::cin >> p >> l;
    assert(!std::cin.fail());
    field = GField(p, l);
    return in;
}

/**
 *
 * @param out
 * @param field
 * @return reference to out stream, containing field print by format
 */
std::ostream& operator<<(std::ostream& out, const GField& field)
{ return (std::cout << "GF(" << field._p << "**" << field._l << ")"); }




