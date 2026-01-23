#include "../include/Math.hpp"

namespace Math {
    float fmod(float a, float b) {
        if (b == 0.0f)
            return getNaN();

        float quotient = a / b;
        float truncated_quotient = static_cast<int>(quotient);

        float remainder = a - truncated_quotient * b;

        return remainder;
    }

    float min(float a, float b) {
        if (isNaN(a) || isNaN(b))
            return getNaN();

        return a < b ? a : b;
    }

    float abs(float a) {
        if (isNaN(a))
            return getNaN();

        return a < 0 ? -a : a;
    }
}