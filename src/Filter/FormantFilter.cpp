#include "Filter.hpp"
#include "Math.hpp"

namespace Filter {
    std::unique_ptr<float[]> apply_formant_filter(const float* samples, int sample_length, float peak, float mix, float spread) {
        if (!samples || sample_length <= 0)
            return nullptr;

        const int num_formants;
        BiquadFilter formant_filters[4];
        std::unique_ptr<float[]> filtered;
        static const float vowel_formants[5][4];
        float q_values[4];
        int vowel_index;
        int f, i;
        float base_freq, spread_factor, formant_freq, q;
        float omega, omega_degrees, sin_omega, cos_omega, alpha;
        float b0, b1, b2, a0, a1, a2;
        float filtered_sample, weight, normalization, formant_output;

        vowel_formants[0][0] = 800.0f; vowel_formants[0][1] = 1150.0f; vowel_formants[0][2] = 2900.0f; vowel_formants[0][3] = 3900.0f;   // "ah"
        vowel_formants[1][0] = 400.0f; vowel_formants[1][1] = 1600.0f; vowel_formants[1][2] = 2700.0f; vowel_formants[1][3] = 3300.0f;   // "eh"
        vowel_formants[2][0] = 350.0f; vowel_formants[2][1] = 1700.0f; vowel_formants[2][2] = 2700.0f; vowel_formants[2][3] = 3700.0f;   // "ee"
        vowel_formants[3][0] = 450.0f; vowel_formants[3][1] = 800.0f; vowel_formants[3][2] = 2830.0f; vowel_formants[3][3] = 3800.0f;    // "oh"
        vowel_formants[4][0] = 325.0f; vowel_formants[4][1] = 700.0f; vowel_formants[4][2] = 2530.0f; vowel_formants[4][3] = 3500.0f;     // "oo"

        q_values[0] = 8.0f;
        q_values[1] = 12.0f;
        q_values[2] = 16.0f;
        q_values[3] = 20.0f;

        num_formants = 4;

        peak = Math::Clamp(peak, 0.0f, 1.0f);
        mix = Math::Clamp(mix, 0.0f, 1.0f);
        spread = Math::Clamp(spread, 0.0f, 1.0f);

        // Reset all filters
        for (f = 0; f < num_formants; f++) {
            formant_filters[f].reset();
        }

        filtered = std::make_unique<float[]>(sample_length);

        for (i = 0; i < sample_length; i++) {
            filtered[i] = 0.0f;
        }

        vowel_index = static_cast<int>(peak * 4.99f);
        if (vowel_index > 4) vowel_index = 4;

        for (f = 0; f < num_formants; f++) {
            base_freq = vowel_formants[vowel_index][f];
            spread_factor = 1.0f + (f * spread * 0.2f);
            formant_freq = base_freq * spread_factor;
            q = q_values[f] * (1.0f + spread * 0.5f);

            omega = 2.0f * Math::PI * formant_freq / Resonix::SAMPLE_RATE;

            omega_degrees = omega * 180.0f / Math::PI;
            sin_omega = Math::Sine(omega_degrees);
            cos_omega = Math::Cosine(omega_degrees);

            alpha = sin_omega / (2.0f * q);
            b0 = alpha;
            b1 = 0.0f;
            b2 = -alpha;
            a0 = 1.0f + alpha;
            a1 = -2.0f * cos_omega;
            a2 = 1.0f - alpha;

            formant_filters[f].setCoefficients(b0/a0, b1/a0, b2/a0, a1/a0, a2/a0);

            for (i = 0; i < sample_length; i++) {
                filtered_sample = formant_filters[f].process(samples[i]);
                weight = 1.0f - (f * 0.15f);
                filtered[i] += filtered_sample * weight;
            }
        }

        normalization = 0.5f;
        for (i = 0; i < sample_length; i++) {
            formant_output = filtered[i] * normalization;
            filtered[i] = samples[i] * (1.0f - mix) + formant_output * mix;
        }

        return filtered;
    }
}