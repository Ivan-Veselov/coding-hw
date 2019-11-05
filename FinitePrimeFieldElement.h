#pragma once

#include <string>
#include <cassert>

#include "numbers.h"

template <Natural P>
class FinitePrimeFieldElement;

template <Natural P>
class FinitePrimeFieldElement {
    private:
        static bool isChecked;

        Whole remainder;

    public:
        FinitePrimeFieldElement()
        : remainder(0) {
            if (!isChecked) {
                check();
            }
        }

        FinitePrimeFieldElement(Whole number)
        : remainder(toRemainderClassMainRepresentative(number)) {
            if (!isChecked) {
                check();
            }
        }

        FinitePrimeFieldElement inv() {
            return fastPower(*this, P - 2);
        }

        FinitePrimeFieldElement& operator += (const FinitePrimeFieldElement &other) {
            remainder += other.remainder;
            if (remainder > P) {
                remainder -= P;
            }

            return *this;
        }

        FinitePrimeFieldElement& operator -= (const FinitePrimeFieldElement &other) {
            remainder -= other.remainder;
            if (remainder < 0) {
                remainder += P;
            }

            return *this;
        }

        FinitePrimeFieldElement& operator *= (const FinitePrimeFieldElement &other) {
            Whole64 tmp = Whole64(remainder) * other.remainder;
            remainder = Whole(tmp % P);

            return *this;
        }

        FinitePrimeFieldElement& operator /= (const FinitePrimeFieldElement &other) {
            return *this *= other.inv();
        }

        std::string prettyString() const {
            return std::to_string(remainder) + " (mod " + std::to_string(P) + ")";
        }

    private:
        static void check() {
            assert(isPrime(P));
        }

        Whole toRemainderClassMainRepresentative(Whole aRemainder) {
            Whole representative = aRemainder % P;
            if (representative < 0) {
                representative += P;
            }

            return representative;
        }
};

template <Natural P>
bool FinitePrimeFieldElement<P>::isChecked = false;

template <Natural P>
FinitePrimeFieldElement<P> operator + (FinitePrimeFieldElement<P> element1, const FinitePrimeFieldElement<P> &element2) {
    return element1 += element2;
}

template <Natural P>
FinitePrimeFieldElement<P> operator - (FinitePrimeFieldElement<P> element1, const FinitePrimeFieldElement<P> &element2) {
    return element1 -= element2;
}

template <Natural P>
FinitePrimeFieldElement<P> operator * (FinitePrimeFieldElement<P> element1, const FinitePrimeFieldElement<P> &element2) {
    return element1 *= element2;
}

template <Natural P>
FinitePrimeFieldElement<P> operator / (FinitePrimeFieldElement<P> element1, const FinitePrimeFieldElement<P> &element2) {
    return element1 /= element2;
}
