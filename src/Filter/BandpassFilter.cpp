#include "Filter.hpp"
#include "Math.hpp"

namespace Filter {
    std::unique_ptr<float[]> apply_bandpass_filter(const float* samples, int sample_length, float center_hz, float bandwidth_hz, float resonance) {
        if (!samples || sample_length <= 0 || center_hz <= 0 || bandwidth_hz <= 0)
            return nullptr;

        auto filtered = std::make_unique<float[]>(sample_length);
        BiquadFilter filter;
        float q, omega, omega_degrees, sin_omega, cos_omega, alpha;
        float b0, b1, b2, a0, a1, a2;

        q = (center_hz / bandwidth_hz);
        if (resonance > 0.707f) {
            q *= (resonance / 0.707f);
        }

        omega = 2.0f * Math::PI * center_hz / Resonix::SAMPLE_RATE;
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