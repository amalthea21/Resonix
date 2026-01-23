#include "../include/Math.hpp"

namespace Math {
    float Sine(float angle) {
        angle = fmod(angle, 360.0f);

        if (angle < 0.0f) angle += 360.0f;

        float radians = angle * PI / 180.0f;

        float term = radians;
        float sum = radians;

        for (int i = 1; i <= 10; i++) {
            term *= -radians * radians / ((2 * i) * (2 * i + 1));
            sum += term;
        }

        return sum;
    }

    float Cosine(float angle) {
        angle = fmod(angle, 360.0f);

        if (angle < 0.0f) angle += 360.0f;

        float radians = angle * PI / 180.0f;

        float term = 1.0f;
        float sum = 1.0f;

        for (int i = 1; i <= 10; i++)
        {
            term *= -(radians * radians) / ((2 * i - 1) * (2 * i));
            sum += term;
        }

        return sum;
    }

    float Tangent(float angle) {
        float cos_val = Cosine(angle);

        if (isNaN(cos_val) || abs(cos_val) < 0.001f)
            return getNaN();

        float result = Sine(angle) / cos_val;

        if (result > 10.0f) return 10.0f;
        if (result < -10.0f) return -10.0f;

        return result;
    }

    float Cotangent(float angle) {
        float sin_val = Sine(angle);

        if (isNaN(sin_val) || abs(sin_val) < 0.001f)
            return getNaN();

        float result = Cosine(angle) / sin_val;

        if (result > 10.0f) return 10.0f;
        if (result < -10.0f) return -10.0f;

        return result;
    }

    float Hann(float n, float N) {
        if (isNaN(n) || isNaN(N))
            return getNaN();

        float cos_arg = 180.0f * n / (N - 1);

        return 0.5f * (1.0f - Cosine(cos_arg));
    }
}