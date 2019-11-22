#ifndef EX1_GFNUMBER_H
#define EX1_GFNUMBER_H
#include "GField.h"

/**
 * represent a number in a galois field.
 */
class GFNumber
{
private:
    long _n;
    GField _field;

    /** add a single factor */
    void _addFactor(GFNumber** result, int* numOfFactors, long n);

    /** return factor of number n, or -1 otherwise */
    long _pollardRho(long n);

    /** update factors of n */
    void _trialDivision(GFNumber** result, int* numOfFactors, long n);

public:
    //constructors & destructor
    /** default constructor */
    GFNumber() : GFNumber(0, GField()) {};

    /** constructor for given number by long representation and field by field representation*/
    GFNumber(long n, const GField& field);

    /** constructor for given number by long representation, and default field*/
    GFNumber(long n) : GFNumber(n, GField()) {};

    /** copy constructor */
    GFNumber(const GFNumber& other) : _n(other.getNumber()), _field(other.getField()){};

    /** default distructor */
    ~GFNumber() = default;

    //member funcs
    /** getter for GFNumber's number */
    long getNumber() const { return _n; };

    /** getter for GFNumber's field */
    GField getField() const { return _field; };

    /** return pointer to an array of GFNumber factors of current number */
    GFNumber* getPrimeFactors(int* numOfFactors);

    /** prints out GFNumber's prime factors */
    void printFactors();

    /** returns true if GFNumber is prime, false otherwise */
    bool getIstPrime() { return GField::isPrime(_n); };

    //overloaded operators
    /**
     *
     * @param other
     * @return default assigning of GFNumber of other GFNumber
     */
    GFNumber& operator=(const GFNumber& other) = default;

    /**
     *
     * @param other
     * @return new GFNumber, after subtraction calculation of cur GFNumber and other GFNumber
     */
    GFNumber operator+(const GFNumber& other);

    /**
     *
     * @param k
     * @return new GFNumber, after subtraction calculation of cur GFNumber and long k
     */
    GFNumber operator+(long k);

    /**
     *
     * @param other
     * @return reference to cur GFNumber, after addition calculation of cur GFNumber and other GFNumber
     */
    GFNumber& operator+=(const GFNumber& other);

    /**
     *
     * @param k
     * @return reference to cur GFNumber, after addition calculation of cur GFNumber and long k
     */
    GFNumber& operator+=(long k);

    /**
     *
     * @param other
     * @return new GFNumber, after subtraction calculation of cur GFNumber and other GFNumber
     */
    GFNumber operator-(const GFNumber& other);

    /**
     *
     * @param k
     * @return new GFNumber, after subtraction calculation of cur GFNumber and long k
     */
    GFNumber operator-(long k);

    /**
     *
     * @param other
     * @return reference to cur GFNumber, after subtraction calculation of other GFNumber
     */
    GFNumber& operator-=(const GFNumber& other);

    /**
     *
     * @param k
     * @return reference to cur GFNumber, after subtraction calculation of long k
     */
    GFNumber& operator-=(long k);

    /**
     *
     * @param other
     * @return new GFNumber, after multiply calculation of cur GFNumber and other GFNumber
     */
    GFNumber operator*(const GFNumber& other);

    /**
     *
     * @param k
     * @return new GFNumber, after multiply calculation of cur GFNumber and long k
     */
    GFNumber operator*(long k);

    /**
     *
     * @param other
     * @return reference to cur GFNumber, after multiply calculation of other GFNumber
     */
    GFNumber& operator*=(const GFNumber& other);

    /**
     *
     * @param k
     * @return reference to cur GFNumber, after multiply calculation of long k
     */
    GFNumber& operator*=(long k);

    /**
     *
     * @param other
     * @return new GFNumber, after modulo calculation of cur GFNumber and other GFNumber
     */
    GFNumber operator%(const GFNumber& other);

    /**
     *
     * @param k
     * @return new GFNumber, after modulo calculation of cur GFNumber and long k
     */
    GFNumber operator%(long k);

    /**
     *
     * @param other
     * @return reference to cur GFNumber, after modulo calculation of other GFNumber
     */
    GFNumber& operator%=(const GFNumber& other);

    /**
     *
     * @param k
     * @return reference to cur GFNumber, after modulo calculation of long k
     */
    GFNumber& operator%=(long k);

    /**
     *
     * @param n
     * @return true if cur GFNumber == given GFNumber n
     */
    bool operator==(const GFNumber& n);

    /**
     *
     * @param n
     * @return true if cur GFNumber != given GFNumber n
     */
    bool operator!=(const GFNumber& n);

    /**
     *
     * @param n
     * @return true if cur GFNumber < given GFNumber n
     */
    bool operator<(const GFNumber& n);

    /**
     *
     * @param n
     * @return true if cur GFNumber <= given GFNumber n
     */
    bool operator<=(const GFNumber& n);

    /**
     *
     * @param n
     * @return true if cur GFNumber > given GFNumber n
     */
    bool operator>(const GFNumber& n);

    /**
     *
     * @param n
     * @return true if cur GFNumber >= given GFNumber n
     */
    bool operator>=(const GFNumber& n);

    //friend functions
    /**
     *
     * @param in
     * @param field
     * @return reference to input stream, after assigning GFNumber values
     */
    friend std::istream& operator>>(std::istream& in, GFNumber& number);

    /**
     *
     * @param out
     * @param number
     * @return reference to out stream, containing GFNumber print by format
     */
    friend std::ostream& operator<<(std::ostream& out, const GFNumber& number);
};

#endif //EX1_GFNUMBER_H
