#include <iostream>

#include "FinitePrimeFieldElement.h"
#include "Polynomial.h"

int main() {
    auto polynomials = Polynomial<GF<3>>::allPolynomials(2);

    for (const auto &polynomial : polynomials) {
        if (polynomial == Polynomial<GF<3>>::ZERO || polynomial[polynomial.deg()] != GF<3>::ONE) {
            continue;
        }

        bool isDivisible = false;
        for (const auto &other : polynomials) {
            if (other.deg() < 1 || other.deg() >= polynomial.deg()) {
                continue;
            }

            if (polynomial % other == Polynomial<GF<3>>::ZERO) {
                std::cout << polynomial.formulaString() << " = (" << other.formulaString() << ") * (" << (polynomial / other).formulaString() << ")" << std::endl;
                isDivisible = true;
                break;
            }
        }

        if (!isDivisible) {
            std::cout << polynomial.formulaString() << " is a prime polynomial" << std::endl;
        }
    }

    return 0;
}