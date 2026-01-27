#include "Math.hpp"

namespace Math {
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