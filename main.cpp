#include <iostream>

#include "FinitePrimeFieldElement.h"
#include "Polynomial.h"

int main() {
    /*GF<7> a, b;

    a = 3;
    b = 6;

    auto c = a.inv();

    std::cout << c.prettyString() << std::endl;*/

    auto x = Polynomial<GF<7>>::X;

    auto pol1 = x(4) + 2 * x(2) + 5 * x(1) + 2;
    auto pol2 = x(1) * 3 - 3 * x(2);

    auto pol3 = pol1 + pol2;

    std::cout << (pol3).prettyString() << std::endl;

    return 0;
}