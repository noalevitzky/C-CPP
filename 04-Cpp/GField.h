#ifndef EX1_GFIELD_H
#define EX1_GFIELD_H

#include <iostream>
#include <cmath>

class GFNumber;

/**
 * represent a galois field of char p and degree l.
 */
class GField
{
private:
    long _p, _l;

public:
    //constructors & destractor
    /** default constructor */
    GField() : GField(2, 1) {};

    /** constructor for given char by long representation, and default degree */
    GField(long p) : GField(p, 1) {};

    /** constructor for given char and degree by long representation */
    GField(long p, long l);

    /** copy constructor */
    GField(const GField& other) : _p(other.getChar()), _l(other.getDegree()) {};

    /** default destructor */
    ~GField() = default;

    //member funcs
    /** getter for field's char */
    long getChar() const { return _p; }

    /** getter for field's degree */
    long getDegree() const { return _l; }

    /** getter for field's order */
    long getOrder() { return ((long) powl((long double) _p, (long double) _l)); }

    /**
     *
     * @param p
     * @return true if given number is prime, false otherwise
     */
    static bool isPrime(const long& p);

    /**
     *
     * @param a
     * @param b
     * @return gcd of 2 given numbers, based on euclidean algorithm
     */
    GFNumber gcd(GFNumber& a, GFNumber& b);

    /**
     *
     * @param k long to be converted
     * @return new GFNumber of given number in current field
     */
    GFNumber createNumber(long k);

    //overloaded operators
    /**
     *
     * @param other
     * @return reference to assigned GField
     */
    GField& operator=(const GField& other) = default;

    /**
    *
    * @param other
    * @return true if current GField is equal to other GField, true otherwise
    */
    bool operator==(const GField& other);

    /**
     *
     * @param other
     * @return true if current GField is unequal to other GField, true otherwise
     */
    bool operator!=(const GField& other);

    //friend functions
    /**
     *
     * @param in
     * @param field
     * @return reference to input stream, after assigning field values
     */
    friend std::istream& operator>>(std::istream& in, GField& field);

    /**
     *
     * @param out
     * @param field
     * @return reference to out stream, containing field print by format
     */
    friend std::ostream& operator<<(std::ostream& out, const GField& field);
};

#endif //EX1_GFIELD_H
