#include "Filter.hpp"

namespace Filter {
    std::unique_ptr<float[]> apply_formant_filter(const float* samples, int sample_length, float peak, float mix, float spread) {
        if (!samples || sample_length <= 0)
            return nullptr;

        if (peak < 0.0f) peak = 0.0f;
        if (peak > 1.0f) peak = 1.0f;
        if (mix < 0.0f) mix = 0.0f;
        if (mix > 1.0f) mix = 1.0f;
        if (spread < 0.0f) spread = 0.0f;
        if (spread > 1.0f) spread = 1.0f;

        const int num_formants = 4;
        BiquadFilter formant_filters[num_formants];
        auto filtered = std::make_unique<float[]>(sample_length);

        static const float vowel_formants[5][4] = {
            {800, 1150, 2900, 3900},   // "ah"
            {400, 1600, 2700, 3300},   // "eh"
            {350, 1700, 2700, 3700},   // "ee"
            {450, 800, 2830, 3800},    // "oh"
            {325, 700, 2530, 3500}     // "oo"
        };

        int vowel_index = static_cast<int>(peak * 4.99f);
        if (vowel_index < 0) vowel_index = 0;
        if (vowel_index > 4) vowel_index = 4;

        float q_values[] = {8.0f, 12.0f, 16.0f, 20.0f};

        for (int f = 0; f < num_formants; f++) {
            float base_freq = vowel_formants[vowel_index][f];
            float spread_factor = 1.0f + (f * spread * 0.2f);
            float formant_freq = base_freq * spread_factor;
            float q = q_values[f] * (1.0f + spread * 0.5f);

            float omega = 2.0f * Math::PI * formant_freq / Resonix::SAMPLE_RATE;
            float omega_degrees = omega * 180.0f / Math::PI;
            float sin_omega = Math::Sine(omega_degrees);
            float cos_omega = Math::Cosine(omega_degrees);
            float alpha = sin_omega / (2.0f * q);

            float b0 = alpha;
            float b1 = 0.0f;
            float b2 = -alpha;
            float a0 = 1.0f + alpha;
            float a1 = -2.0f * cos_omega;
            float a2 = 1.0f - alpha;

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
            formant_output *= 4.0f;
            filtered[i] = sample * (1.0f - mix) + formant_output * mix;
        }

        return filtered;
    }
}