#include <cmath>
#include <iostream>
#include "Fractal.h"
#include <vector>
#define FULL '#'

/**
 * print out fractal
 */
void Fractal::printFractal()
{
    //init matrix
    size_t size;
    std::vector<std::vector<char>> matrix;

    //update matrix by pattern
    getMatrix(matrix, size);

    //print matrix
    for (size_t i = 0; i < size; i++)
    {
        for (size_t j = 0; j < size; j++)
        {
            std::cout << matrix[i][j];
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

/**
 * getter for matrix. update size of matrix as well as filling matrix by pattern
 * @param matrix
 * @param size num of cells
 */
void Fractal::getMatrix(std::vector<std::vector<char>>& matrix, size_t& size)
{
    //create matrix
    size = pow(getPattern().size(), _dim);
    matrix = std::vector<std::vector<char>>(size, std::vector<char>(size, ' '));

    //fill in matrix
    _fillMatrix(_dim, 0, 0, matrix);
}

/**
 * fills in the matrix based on pattern, recursively
 * @param dim
 * @param x block row index
 * @param y block column index
 * @param matrix
 */
void Fractal::_fillMatrix(const size_t dim, const size_t x, const size_t y,
                          std::vector<std::vector<char>>& matrix)
{
    if (dim == 1)
    {
        for (size_t i = 0; i < getPattern().size(); i++)
        {
            for (size_t j = 0; j < getPattern().size(); j++)
            {
                if (getPattern()[i][j] == FULL)
                {
                    size_t first = x + i;
                    size_t second = y + j;
                    matrix[first][second] = FULL;
                }
            }
        }
    }
    else
    {
        for (size_t i = 0; i < getPattern().size(); i++)
        {
            for (size_t j = 0; j < getPattern().size(); j++)
            {
                if (getPattern()[i][j] == FULL)
                {
                    _fillMatrix(dim - 1, x + i * (int) pow(getPattern().size(),
                            (int) dim - 1), y + j * (int) pow(getPattern().size(),
                                    (int) dim - 1), matrix);
                }
            }
        }
    }
}