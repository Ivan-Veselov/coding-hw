#include <iostream>

#include "FinitePrimeFieldElement.h"

int main() {
    FinitePrimeFieldElement<7> a, b;

    a = 3;
    b = 6;

    auto c = a.inv();

    std::cout << c.prettyString() << std::endl;

    return 0;
}