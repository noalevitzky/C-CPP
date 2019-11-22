#ifndef EX2_FRACTAL_H
#define EX2_FRACTAL_H

#include <utility>
#include <vector>

/**
 * abstruct class (pure virtual). implement rule of 3 since there's no need for move semantics
 */
class Fractal
{
protected:
    size_t _dim;

    /**
     * fills in the matrix based on pattern, recursively
     * @param dim
     * @param x block row index
     * @param y block column index
     * @param matrix
     */
    void _fillMatrix(size_t dim, size_t x, size_t y, std::vector<std::vector<char>>& matrix);

public:
    /**
     * ctor
     * @param t
     * @param d
     */
    explicit Fractal(size_t d) : _dim(d)
    {};

    /**
     * default copy ctor
     * @param other
     */
    Fractal(const Fractal& other) = default;

    /**
     * default dtor
     */
    virtual ~Fractal() = default;

    /**
     * default copy operator=
     * @param other
     * @return
     */
    Fractal& operator=(const Fractal& other) = default;

    /**
     * print out fractal
     */
    void printFractal();

    /**
     * getter for fractal metrix
     * @param matrix
     * @param size num of cells
     */
    void getMatrix(std::vector<std::vector<char>>& matrix, size_t& size);

    /**
     *
     * @return fractal pattern in a vector form
     */
    virtual std::vector<std::vector<char>> getPattern() = 0;
};

/**
 * SierpinskiCarpet fractal
 */
class SierpinskiCarpet : public Fractal
{
public:
    /**
     * ctor
     * @param d
     */
    explicit SierpinskiCarpet(size_t d) : Fractal(d)
    {};

    /**
     * default copy ctor
     * @param other
     */
    SierpinskiCarpet(const SierpinskiCarpet& other) = default;

    /**
     * default dtor
     */
    ~SierpinskiCarpet() override = default;

    /**
     * default copy operator=
     * @param other
     * @return
     */
    SierpinskiCarpet& operator=(const SierpinskiCarpet& other) = default;

    /**
     * default move operator=
     * @param other
     * @return
     */
    SierpinskiCarpet& operator=(SierpinskiCarpet && other) = default;

    /**
     *
     * @return fractal pattern in a vector form
     */
    std::vector<std::vector<char>> getPattern() override
    {
        return {{'#', '#', '#'},
                {'#', ' ', '#'},
                {'#', '#', '#'}};
    }
};

/**
 * SierpinskiSieve fractal
 */
class SierpinskiSieve : public Fractal
{
public:
    /**
     * ctor
     * @param d
     */
    explicit SierpinskiSieve(size_t d) : Fractal(d)
    {};

    /**
    * default copy ctor
    * @param other
    */
    SierpinskiSieve(const SierpinskiSieve& other) = default;

    /**
     * default dtor
     */
    ~SierpinskiSieve() override = default;

    /**
    * default copy operator=
    * @param other
    * @return
    */
    SierpinskiSieve& operator=(const SierpinskiSieve& other) = default;

    /**
    * default move operator=
    * @param other
    * @return
    */
    SierpinskiSieve& operator=(SierpinskiSieve && other) = default;

    /**
     *
     * @return fractal pattern in a vector form
     */
    std::vector<std::vector<char>> getPattern() override
    {
        return {{'#', '#'},
                {'#', ' '}};
    }
};

/**
 * CantorDust fractal
 */
class CantorDust : public Fractal
{
public:
    /**
     * ctor
     * @param d
     */
    explicit CantorDust(size_t d) : Fractal(d)
    {};

    /**
     * default copy ctor
     * @param other
     */
    CantorDust(const CantorDust& other) = default;

    /**
     * default dtor
     */
    ~CantorDust() override = default;

    /**
    * default copy operator=
    * @param other
    * @return
    */
    CantorDust& operator=(const CantorDust& other) = default;

    /**
    * default move operator=
    * @param other
    * @return
    */
    CantorDust& operator=(CantorDust && other) = default;

    /**
     *
     * @return fractal pattern in a vector form
     */
    std::vector<std::vector<char>> getPattern() override
    {
        return {{'#', ' ', '#'},
                {' ', ' ', ' '},
                {'#', ' ', '#'}};
    }
};

#endif //EX2_FRACTAL_H
