#include "../include/Math.hpp"

namespace Math {
    float Sine(float degrees) {
        float angle, radians;

        auto reduce_angle = [](float a) -> float {
            a = Math::fmod(a, 360.0f);
            return a < 0.0f ? a + 360.0f : a;
        };

        angle = reduce_angle(degrees);
        radians = angle * DEG_TO_RAD;

        auto poly_sin = [](float x) -> float {
            float x2;

            x = Math::fmod(x + PI, TWO_PI) - PI;

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
            a = Math::fmod(a, 360.0f);
            return a < 0.0f ? a + 360.0f : a;
        };

        angle = reduce_angle(degrees);
        return Sine(angle + 90.0f);
    }

    float Tangent(float degrees) {
        float mod180, radians, x, x2, x4, x6, num, den;

        degrees = Math::fmod(degrees, 360.0f);
        if (degrees < 0.0f) degrees += 360.0f;

        mod180 = Math::fmod(degrees, 180.0f);
        if (Math::abs(mod180 - 90.0f) < 0.1f) {
            return getNaN();
        }

        radians = degrees * DEG_TO_RAD;
        radians = Math::fmod(radians + PI, TWO_PI) - PI;

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

        if (Math::abs(den) < EPSILON) {
            return getNaN();
        }

        return num / den;
    }

    float Cotangent(float degrees) {
        float x, x2, x4, numerator, denominator, radians, mod180;

        degrees = Math::fmod(degrees, 360.0f);
        if (degrees < 0.0f) degrees += 360.0f;

        mod180 = Math::fmod(degrees, 180.0f);
        if (Math::abs(mod180) < 0.1f) {
            return getNaN();
        }

        radians = degrees * DEG_TO_RAD;

        if (radians > PI/2) {
            radians -= PI;
        } else if (radians < -PI/2) {
            radians += PI;
        }

        x = radians;

        if (Math::abs(x) < 0.001f) {
            return getNaN();
        }

        x2 = x * x;
        x4 = x2 * x2;

        numerator = 945.0f - 105.0f * x2 + x4;
        denominator = x * (945.0f - 420.0f * x2 + 15.0f * x4);

        if (Math::abs(denominator) < EPSILON) {
            return getNaN();
        }

        return numerator / denominator;
    }
}