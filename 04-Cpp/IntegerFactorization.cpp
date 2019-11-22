#include <iostream>
#include "GFNumber.h"
#include <cassert>

/**
 * program that gets 2 numbers and prints out their operators
 * @return 0 upon success;
 */
int main()
{
    GFNumber a, b;
    std::cin >> a >> b;
    assert(!std::cin.fail());
    std::cout << a + b << "\n" << a - b << "\n" << b - a << "\n" << a * b << "\n";
    a.printFactors();
    b.printFactors();
    return 0;
}
