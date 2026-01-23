#include "Filter.hpp"


namespace Filter {
    struct BiquadFilter {
        float b0, b1, b2;
        float a1, a2;
        float x1, x2;
        float y1, y2;

        BiquadFilter() : x1(0), x2(0), y1(0), y2(0) {}

        float process(float sample) {
            float y = b0 * sample + b1 * x1 + b2 * x2 - a1 * y1 - a2 * y2;

            x2 = x1;
            x1 = sample;
            y2 = y1;
            y1 = y;

            return y;
        }
    };

    float* apply_lowpass_filter(const float* samples, int sample_length, float cutoff_hz, float resonance) {
        if (!samples || sample_length <= 0 || cutoff_hz <= 0)
            return nullptr;

        float* filtered = new float[sample_length];

        BiquadFilter filter;

        float omega = 2.0f * Math::PI * cutoff_hz / Resonix::SAMPLE_RATE;
        float alpha = Math::Sine(omega) / (2.0f * resonance);

        float cos_omega = Math::Cosine(omega);

        float b0 = (1.0f - cos_omega) / 2.0f;
        float b1 = 1.0f - cos_omega;
        float b2 = b0;
        float a0 = 1.0f + alpha;
        float a1 = -2.0f * cos_omega;
        float a2 = 1.0f - alpha;

        filter.b0 = b0 / a0;
        filter.b1 = b1 / a0;
        filter.b2 = b2 / a0;
        filter.a1 = a1 / a0;
        filter.a2 = a2 / a0;

        for (int i = 0; i < sample_length; i++) {
            filtered[i] = filter.process(samples[i]);
        }

        return filtered;
    }
}
