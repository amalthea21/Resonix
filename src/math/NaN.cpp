#include "../include/Math.hpp"

namespace Math {
    float getNaN() {
        // IEEE 754 quiet NaN: sign=0, exponent=all 1s, mantissa non-zero
        unsigned int nan_bits = 0x7FC00000;
        return *reinterpret_cast<float*>(&nan_bits);
    }

    bool isNaN(float a) {
        return(a != a);
    }
}