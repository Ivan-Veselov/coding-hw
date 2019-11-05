#include "numbers.h"

bool isPrime(Natural number) {
    if (number < 2) {
        return false;
    }

    for (Natural divisor = 2; Natural64(divisor) * Natural64(divisor) <= number; ++divisor) {
        if (number % divisor == 0) {
            return false;
        }
    }

    return true;
}
