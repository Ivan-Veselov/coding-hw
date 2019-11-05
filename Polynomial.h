#pragma once

#include <vector>

#include "numbers.h"

template <typename Coefficient>
class Polynomial {
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

        Polynomial& operator += (const Polynomial &other) {
            const auto numOfConsideredCoefficients = other.coefficients.size();
            addZeroCoefficientsUpTo(numOfConsideredCoefficients);

            for (auto i = 0; i < numOfConsideredCoefficients; ++i) {
                coefficients[i] += other.coefficients[i];
            }

            normalize();
            return *this;
        }

        Polynomial& operator -= (const Polynomial &other) {
            const auto numOfConsideredCoefficients = other.coefficients.size();
            addZeroCoefficientsUpTo(numOfConsideredCoefficients);

            for (auto i = 0; i < numOfConsideredCoefficients; ++i) {
                coefficients[i] -= other.coefficients[i];
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

        std::string prettyString() const {
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


            result += ", a_i from " + Coefficient::setPrettyString();
            return result;
        }

    private:
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

namespace Polynomials {
    template <typename Coefficient>
    Polynomial<Coefficient> X(const Natural &power) {
        return Polynomial<Coefficient>(power);
    }
}
