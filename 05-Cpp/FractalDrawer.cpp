#include <iostream>
#include <fstream>
#include <vector>
#include "Fractal.h"
#include <string>
#include <stack>
#define NUM_ARGS 2
#define FILE_PATH 1
#define COMMA ','
#define LOW_DIM 1
#define HIGH_DIM 6
#define MEMORY_FAIL -2

/**
 * process a single line. delete newly created fractal!
 * @param line
 * @return if successful
 */
int process(std::string &line, std::stack<Fractal*>& all)
{
    //validate num of columns
    if (line.size() != 3 || line[1] != COMMA)
    {
        return EXIT_FAILURE;
    }

    size_t type = line[0] - '0';
    size_t dim = line[2] - '0';

    //validate dim
    if (dim < LOW_DIM || dim > HIGH_DIM)
    {
        return EXIT_FAILURE;
    }

    //create fractal
    Fractal* f;
    try
    {
        switch (type)
        {
            case 1:
                f = new SierpinskiCarpet(dim);
                all.push(f);
                break;
            case 2:
                f = new SierpinskiSieve(dim);
                all.push(f);
                break;
            case 3:
                f = new CantorDust(dim);
                all.push(f);
                break;
            default:
                //type is invalid
                return EXIT_FAILURE;
        }
    }
    catch (const std::bad_alloc& e)
    {
        return MEMORY_FAIL;
    }
    return EXIT_SUCCESS;
}

/**
 * program that prints out fractals, based on specifications in CSV file.
 * @return is successful
 */
int main(int argc, char** argv)
{
    //validate args num
    if (argc != NUM_ARGS)
    {
        std::cerr << "Usage: FractalDrawer <file path>\n";
        return EXIT_FAILURE;
    }

    // read file
    std::fstream file(argv[FILE_PATH]);

    //validate file exists
    if (!file.good())
    {
        std::cerr << "Invalid input\n";
        file.close();
        return EXIT_FAILURE;
    }

    //validate file is not empty
    else if (file.peek() == file.eof())
    {
        file.close();
        return EXIT_SUCCESS;
    }

    //init container for all fractals to be printed
    std::stack<Fractal*> all;

    //parse file
    std::string line;
    while (std::getline(file, line))
    {
        if (line[line.size() - 1] == '\r')
        {
            line.erase((line.size() - 1));
        }

        int err = process(line, all);
        if (err == MEMORY_FAIL)
        {
            std::cout << "Memory allocation failed\n";
            return EXIT_FAILURE;
        }
        else if (err == EXIT_FAILURE)
        {
            file.close();
            std::cerr << "Invalid input\n";
            return EXIT_FAILURE;
        }
    }
    file.close();

    //print shapes
    while (!all.empty())
    {
        Fractal* f = all.top();
        f->printFractal();
        delete f;
        all.pop();
    }
    return EXIT_SUCCESS;
}

