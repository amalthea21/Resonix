#include "Filter.hpp"

namespace Filter {
    float* apply_highpass_filter(const float* samples, int sample_length, float cutoff_hz, float resonance) {
        if (!samples || sample_length <= 0 || cutoff_hz <= 0)
            return nullptr;

        float* filtered = new float[sample_length];

        BiquadFilter filter;

        float omega = 2.0f * Math::PI * cutoff_hz / Resonix::SAMPLE_RATE;

        // Convert radians to degrees for Math::Sine and Math::Cosine
        float omega_degrees = omega * 180.0f / Math::PI;
        float alpha = Math::Sine(omega_degrees) / (2.0f * resonance);
        float cos_omega = Math::Cosine(omega_degrees);

        float b0 = (1.0f + cos_omega) / 2.0f;
        float b1 = -(1.0f - cos_omega);
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