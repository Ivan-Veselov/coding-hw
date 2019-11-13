#pragma once

#include <cstdint>

typedef uint32_t Natural32;
typedef uint64_t Natural64;
typedef Natural32 Natural;

typedef int32_t Whole32;
typedef int64_t Whole64;
typedef Whole32 Whole;

bool isPrime(Natural number);

template<typename Number>
Number multiplicationNeutralElement() {
    return Number(1);
}

template <typename Number>
Number fastPower(Number value, Natural power) {
    if (power == 0) {
        return multiplicationNeutralElement<Number>();
    }

    if (power == 1) {
        return value;
    }

    Number result = fastPower(value, power / 2);
    result *= result;

    if (power % 2 == 1) {
        result *= value;
    }

    return result;
}
