#include "../include/Math.hpp"

namespace Math {
    float fmod(float a, float b) {
        if (b == 0.0f) {
            // IEEE 754 NaN: exponent all 1s, mantissa non-zero
            unsigned int nan_bits = 0x7FC00000;
            return *reinterpret_cast<float*>(&nan_bits);
        }

        float quotient = a / b;
        float truncated_quotient = static_cast<int>(quotient);

        float remainder = a - truncated_quotient * b;

        return remainder;
    }

    float min(float a, float b) {
        return a > b ? a : b;
    }

    float abs(float a) {
        return a < 0 ? -a : a;
    }
}