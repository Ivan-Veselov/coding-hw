#include <iostream>

#include "FinitePrimeFieldElement.h"
#include "Polynomial.h"

void findAllPrimePolynomials() {
    auto polynomials = Polynomial<GF<2>>::allPolynomials(3);

    for (const auto &polynomial : polynomials) {
        if (polynomial == Polynomial<GF<2>>::ZERO || polynomial[polynomial.deg()] != GF<2>::ONE) {
            continue;
        }

        bool isDivisible = false;
        for (const auto &other : polynomials) {
            if (other.deg() < 1 || other.deg() >= polynomial.deg()) {
                continue;
            }

            if (polynomial % other == Polynomial<GF<2>>::ZERO) {
                std::cout << polynomial.formulaString() << " = (" << other.formulaString() << ") * (" << (polynomial / other).formulaString() << ")" << std::endl;
                isDivisible = true;
                break;
            }
        }

        if (!isDivisible) {
            std::cout << polynomial.formulaString() << " is a prime polynomial" << std::endl;
        }
    }
}

void solve() {
    // x^3 + x + 1 for 2**3
    // x^2 + x + 2 for 3**2

    auto polynomials = Polynomial<GF<3>>::allPolynomials(1);

    auto x = Polynomial<GF<3>>::X;
    auto prime = x(2) + x(1) + 2;

    Natural power = 3 * 3 - 2;

    for (const auto &elem : polynomials) {
        std::cout << "The inverse of " << elem.formulaString() << " is " << std::endl;
        std::cout << "(" << elem.formulaString() << ")**" << power << " = " << std::endl;

        auto inverse = elem;
        for (int i = 0; i < power - 1; ++i) {
            inverse *= elem;
        }

        inverse %= prime;

        std::cout << inverse.formulaString() << std::endl;
        std::cout << "The product is " << ((elem * inverse) % prime).formulaString() << "\n" << std::endl;
    }
}

int main() {
    solve();

    /*auto x = Polynomial<GF<2>>::X;
    auto prime = x(3) + x(1) + 1;

    auto pol = x(1) + 1;
    std::cout << ((pol * pol * pol * pol * pol * pol) % prime).prettyString() << std::endl;
    std::cout << ((fastPower(pol, 6)) % prime).prettyString() << std::endl;*/

    return 0;
}