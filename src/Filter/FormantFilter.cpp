#include "Filter.hpp"

namespace Filter {
    std::unique_ptr<float[]> apply_formant_filter(const float* samples, int sample_length, float peak, float mix, float spread) {
        if (!samples || sample_length <= 0)
            return nullptr;

        const int num_formants = 4;
        int vowel_index;
        BiquadFilter formant_filters[num_formants];
        auto filtered = std::make_unique<float[]>(sample_length);

        float base_freq, spread_factor, formant_freq, q, omega, omega_degrees, alpha, cos_omega,
              b0, b1, b2, a0, a1, a2;

        float q_values[] = {8.0f, 12.0f, 16.0f, 20.0f};

        static const float vowel_formants[5][4] = {
            {800, 1150, 2900, 3900},
            {400, 1600, 2700, 3300},
            {350, 1700, 2700, 3700},
            {450, 800, 2830, 3800},
            {325, 700, 2530, 3500}
        };

        vowel_index = static_cast<int>(peak * 4.99f);
        vowel_index = Math::clamp(vowel_index, 0, 4);

        for (int f = 0; f < num_formants; f++) {
            base_freq = vowel_formants[vowel_index][f];
            spread_factor = 1.0f + (f * spread * 0.2f);
            formant_freq = base_freq * spread_factor;
            q = q_values[f] * (1.0f + spread * 0.5f);
            omega = 2.0f * Math::PI * formant_freq / Resonix::SAMPLE_RATE;
            omega_degrees = omega * 180.0f / Math::PI;
            alpha = Math::Sine(omega_degrees) / (2.0f * q);
            cos_omega = Math::Cosine(omega_degrees);

            b0 = alpha;
            b1 = 0.0f;
            b2 = -alpha;
            a0 = 1.0f + alpha;
            a1 = -2.0f * cos_omega;
            a2 = 1.0f - alpha;

            formant_filters[f].b0 = b0 / a0;
            formant_filters[f].b1 = b1 / a0;
            formant_filters[f].b2 = b2 / a0;
            formant_filters[f].a1 = a1 / a0;
            formant_filters[f].a2 = a2 / a0;
        }

        for (int i = 0; i < sample_length; i++) {
            float sample = samples[i];
            float formant_output = sample;

            for (int f = 0; f < num_formants; f++) {
                formant_output = formant_filters[f].process(formant_output);
            }

            filtered[i] = sample * (1.0f - mix) + formant_output * mix;
        }

        return filtered;
    }
}