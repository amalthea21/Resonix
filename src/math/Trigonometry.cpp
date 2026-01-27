#include "../include/Math.hpp"
#include <cmath>

namespace Math {
    float Sine(float degrees) {
        float angle, radians;

        auto reduce_angle = [](float a) -> float {
            a = std::fmod(a, 360.0f);
            return a < 0.0f ? a + 360.0f : a;
        };

        angle = reduce_angle(degrees);
        radians = angle * DEG_TO_RAD;

        auto poly_sin = [](float x) -> float {
            float x2;

            x = std::fmod(x + PI, TWO_PI) - PI;

            constexpr float c7 = 0.9999966f;
            constexpr float c5 = -0.16664824f;
            constexpr float c3 = 0.00830629f;
            constexpr float c1 = -0.00018363f;

            x2 = x * x;
            return x * (c7 + x2 * (c5 + x2 * (c3 + x2 * c1)));
        };

        return poly_sin(radians);
    }

    float Cosine(float degrees) {
        float angle;

        auto reduce_angle = [](float a) -> float {
            a = std::fmod(a, 360.0f);
            return a < 0.0f ? a + 360.0f : a;
        };

        angle = reduce_angle(degrees);
        return Sine(angle + 90.0f);
    }

    float Tangent(float degrees) {
        float mod180, radians, x, x2, x4, x6, num, den;

        degrees = std::fmod(degrees, 360.0f);
        if (degrees < 0.0f) degrees += 360.0f;

        mod180 = std::fmod(degrees, 180.0f);
        if (std::abs(mod180 - 90.0f) < 0.1f) {
            return getNaN();
        }

        radians = degrees * DEG_TO_RAD;
        radians = std::fmod(radians + PI, TWO_PI) - PI;

        x = radians;

        if (x > PI/2) {
            x -= PI;
        } else if (x < -PI/2) {
            x += PI;
        }

        x2 = x * x;
        x4 = x2 * x2;
        x6 = x4 * x2;

        num = x * (-135135.0f + x2 * (17325.0f + x2 * (-378.0f + x2)));
        den = -135135.0f + x2 * (62370.0f + x2 * (-3150.0f + 28.0f * x2));

        if (std::abs(den) < EPSILON) {
            return getNaN();
        }

        return num / den;
    }

    float Cotangent(float degrees) {
        float x, x2, x4, numerator, denominator, radians, mod180;

        degrees = std::fmod(degrees, 360.0f);
        if (degrees < 0.0f) degrees += 360.0f;

        mod180 = std::fmod(degrees, 180.0f);
        if (std::abs(mod180) < 0.1f) {
            return getNaN();
        }

        radians = degrees * DEG_TO_RAD;

        if (radians > PI/2) {
            radians -= PI;
        } else if (radians < -PI/2) {
            radians += PI;
        }

        x = radians;

        if (std::abs(x) < 0.001f) {
            return getNaN();
        }

        x2 = x * x;
        x4 = x2 * x2;

        numerator = 945.0f - 105.0f * x2 + x4;
        denominator = x * (945.0f - 420.0f * x2 + 15.0f * x4);

        if (std::abs(denominator) < EPSILON) {
            return getNaN();
        }

        return numerator / denominator;
    }

    float Hann(float n, float N) {
        float cos_arg, last_N, inv_N_minus_1;
        static float static_last_N, static_inv_N_minus_1;

        if (isNaN(n) || isNaN(N) || N <= 1.0f) {
            return getNaN();
        }

        static_last_N = -1.0f;
        static_inv_N_minus_1 = 0.0f;

        if (N != static_last_N) {
            static_last_N = N;
            static_inv_N_minus_1 = 1.0f / (N - 1.0f);
        }

        cos_arg = 360.0f * n * static_inv_N_minus_1;

        return 0.5f * (1.0f - Cosine(cos_arg));
    }
}