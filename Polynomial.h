#pragma once

#include <vector>

#include "numbers.h"

template <typename Coefficient>
class Polynomial;

template <typename Coefficient>
bool operator == (const Polynomial<Coefficient>&, const Polynomial<Coefficient>&);

template <typename Coefficient>
bool operator != (const Polynomial<Coefficient>&, const Polynomial<Coefficient>&);

template <typename Coefficient>
class Polynomial {
    friend bool operator == <>(const Polynomial<Coefficient>&, const Polynomial<Coefficient>&);
    friend bool operator != <>(const Polynomial<Coefficient>&, const Polynomial<Coefficient>&);

    private:
        std::vector<Coefficient> coefficients;
        // coefficients[i] is a coefficient for x^i

    public:
        class Variable {
            public:
                Polynomial<Coefficient> operator() (const Natural &power) const {
                    return Polynomial<Coefficient>(power);
                }
        };

        static const Variable X;

    public:
        static const Polynomial ZERO;
        static const Polynomial ONE;

    public:
        Polynomial()
        : coefficients({Coefficient::ZERO}) {
        }

        Polynomial(const Coefficient &scalar)
        : coefficients({scalar}) {
        }

        explicit Polynomial(const Natural &power)
        : coefficients(power + 1, Coefficient::ZERO) {
            coefficients[power] = Coefficient::ONE;
        }

        static std::vector<Polynomial> allPolynomials(Natural maxDeg) {
            auto allCoefficients = Coefficient::allElements();

            std::vector<Polynomial> polynomials;

            if (maxDeg == 0) {
                for (const auto &coefficient : allCoefficients) {
                    polynomials.push_back(Polynomial(coefficient));
                }

                return polynomials;
            }

            std::vector<Polynomial> lowerDegreePolynomials = allPolynomials(maxDeg - 1);
            polynomials.insert(polynomials.end(), lowerDegreePolynomials.begin(), lowerDegreePolynomials.end());

            for (const auto &coefficient : allCoefficients) {
                if (coefficient == Coefficient::ZERO) {
                    continue;
                }

                for (auto lowerPolynomial : lowerDegreePolynomials) {
                    lowerPolynomial += Polynomial(maxDeg) * coefficient;
                    polynomials.push_back(lowerPolynomial);
                }
            }

            return polynomials;
        }

        Natural deg() const {
            return coefficients.size() - 1;
        }

        const Coefficient& operator [] (const Natural &degree) const {
            return coefficients[degree];
        }

        Polynomial& operator += (const Polynomial &other) {
            const auto numOfConsideredCoefficients = other.coefficients.size();
            addZeroCoefficientsUpTo(numOfConsideredCoefficients);

            for (auto i = 0; i < numOfConsideredCoefficients; ++i) {
                coefficients[i] += other[i];
            }

            normalize();
            return *this;
        }

        Polynomial& operator -= (const Polynomial &other) {
            const auto numOfConsideredCoefficients = other.coefficients.size();
            addZeroCoefficientsUpTo(numOfConsideredCoefficients);

            for (auto i = 0; i < numOfConsideredCoefficients; ++i) {
                coefficients[i] -= other[i];
            }

            normalize();
            return *this;
        }

        Polynomial& operator *= (const Coefficient &scalar) {
            for (auto &c : coefficients) {
                c *= scalar;
            }

            normalize();
            return *this;
        }

        Polynomial& operator *= (const Polynomial &other) {
            const auto numOfConsideredCoefficients = deg() + other.deg() + 1;
            addZeroCoefficientsUpTo(numOfConsideredCoefficients);

            for (int i = numOfConsideredCoefficients - 1; i >= 0; --i) {
                // i is an index of resulting multiplication
                for (int j = std::min(i, int(deg())); j >= 0 && i - j <= other.deg(); --j) {
                    // j is an index of original polynomial

                    if (j != i) {
                        coefficients[i] += coefficients[j] * other[i - j];
                    } else {
                        coefficients[i] = coefficients[j] * other[i - j];
                    }
                }
            }

            normalize();
            return *this;
        }

        // this may be passed as a 'remainder' or 'quotient'
        void divide(const Polynomial &divisor, Polynomial &quotient, Polynomial &remainder) const {
            assert(divisor != ZERO);

            remainder = *this;

            if (deg() < divisor.deg()) {
                quotient = ZERO;
                return;
            }

            Natural divisorDeg = divisor.deg();
            Natural quotientPower = deg() - divisorDeg + 1;
            quotient.coefficients.resize(quotientPower);

            Coefficient divisorHighCoefficientInv = divisor.coefficients.back().inv();

            do {
                --quotientPower;

                Coefficient quotientCoefficient = remainder[quotientPower + divisorDeg] * divisorHighCoefficientInv;
                quotient[quotientPower] = quotientCoefficient;

                for (auto i = 0; i <= divisorDeg; ++i) {
                    remainder[quotientPower + i] -= divisor[i] * quotientCoefficient;
                }

            } while (quotientPower != 0);

            remainder.normalize();
        }

        Polynomial& operator /= (const Polynomial &other) {
            Polynomial remainder;
            divide(other, *this, remainder);

            return *this;
        }

        Polynomial& operator %= (const Polynomial &other) {
            Polynomial quotient;
            divide(other, quotient, *this);

            return *this;
        }

        std::string formulaString() const {
            std::vector<std::string> terms;
            for (auto power = coefficients.size() - 1; power > 0; --power) {
                auto c = coefficients[power];
                if (c == Coefficient::ZERO) {
                    continue;
                }

                std::string termStr;
                if (c != Coefficient::ONE) {
                    termStr = c.formulaString() + " * ";
                }

                termStr += "x";

                if (power > 1) {
                    termStr += "^" + std::to_string(power);
                }

                terms.push_back(termStr);
            }

            auto bias = coefficients[0];
            if (bias != Coefficient::ZERO || terms.empty()) {
                terms.push_back(bias.formulaString());
            }

            std::string result;
            for (auto i = 0; i < terms.size(); ++i) {
                if (i > 0) {
                    result += " + ";
                }

                result += terms[i];
            }

            return result;
        }

        std::string prettyString() const {
            return formulaString() + ", a_i is from " + Coefficient::setPrettyString();
        }

    private:
        Coefficient& operator [] (const Natural &degree) {
            return coefficients[degree];
        }

        void normalize() {
            while (coefficients.back() == Coefficient::ZERO && coefficients.size() > 1) {
                coefficients.pop_back();
            }
        }

        void addZeroCoefficientsUpTo(const Natural numberOfCoefficients) {
            if (numberOfCoefficients > coefficients.size()) {
                coefficients.resize(numberOfCoefficients, Coefficient::ZERO);
            }
        }
};

template <typename Coefficient>
const Polynomial<Coefficient> Polynomial<Coefficient>::ZERO = Coefficient::ZERO;

template <typename Coefficient>
const Polynomial<Coefficient> Polynomial<Coefficient>::ONE  = Coefficient::ONE;

template <typename Coefficient>
Polynomial<Coefficient> operator + (Polynomial<Coefficient> element1, const Polynomial<Coefficient> &element2) {
    return element1 += element2;
}

template <typename Coefficient>
Polynomial<Coefficient> operator - (Polynomial<Coefficient> element1, const Polynomial<Coefficient> &element2) {
    return element1 -= element2;
}

template <typename Coefficient, typename Scalar>
Polynomial<Coefficient> operator + (Polynomial<Coefficient> element, const Scalar &scalar) {
    return element += Coefficient(scalar);
}

template <typename Coefficient, typename Scalar>
Polynomial<Coefficient> operator + (const Scalar &scalar, Polynomial<Coefficient> element) {
    return element += Coefficient(scalar);
}

template <typename Coefficient, typename Scalar>
Polynomial<Coefficient> operator - (Polynomial<Coefficient> element, const Scalar &scalar) {
    return element -= Coefficient(scalar);
}

template <typename Coefficient, typename Scalar>
Polynomial<Coefficient> operator - (const Scalar &scalar, Polynomial<Coefficient> element) {
    return element -= Coefficient(scalar);
}

template <typename Coefficient, typename Scalar>
Polynomial<Coefficient> operator * (Polynomial<Coefficient> element, const Scalar &scalar) {
    return element *= scalar;
}

template <typename Coefficient, typename Scalar>
Polynomial<Coefficient> operator * (const Scalar &scalar, Polynomial<Coefficient> element) {
    return element *= scalar;
}

template <typename Coefficient>
Polynomial<Coefficient> operator * (Polynomial<Coefficient> element1, const Polynomial<Coefficient> &element2) {
    return element1 *= element2;
}

template <typename Coefficient>
Polynomial<Coefficient> operator / (Polynomial<Coefficient> element1, const Polynomial<Coefficient> &element2) {
    return element1 /= element2;
}

template <typename Coefficient>
Polynomial<Coefficient> operator % (Polynomial<Coefficient> element1, const Polynomial<Coefficient> &element2) {
    return element1 %= element2;
}

template <typename Coefficient>
bool operator == (const Polynomial<Coefficient> &element1, const Polynomial<Coefficient> &element2) {
    return element1.coefficients == element2.coefficients;
}

template <typename Coefficient>
bool operator != (const Polynomial<Coefficient> &element1, const Polynomial<Coefficient> &element2) {
    return element1.coefficients != element2.coefficients;
}

namespace Polynomials {
    template <typename Coefficient>
    Polynomial<Coefficient> X(const Natural &power) {
        return Polynomial<Coefficient>(power);
    }
}

/*
https://www.fluentcpp.com/2017/08/15/function-templates-partial-specialization-cpp/

template <typename Coefficient>
Polynomial<Coefficient> multiplicationNeutralElement() {
    return Polynomial<Coefficient>(Coefficient::ONE);
}
*/
