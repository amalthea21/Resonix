#pragma once

#include <memory>
#include "Math.hpp"
#include "Resonix.hpp"

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

    std::unique_ptr<float[]> apply_lowpass_filter(const float* samples, int sample_length, float cutoff_hz, float resonance);

    std::unique_ptr<float[]> apply_highpass_filter(const float* samples, int sample_length, float cutoff_hz, float resonance);

    std::unique_ptr<float[]> apply_formant_filter(const float* samples, int sample_length, float peak, float mix, float spread);
}