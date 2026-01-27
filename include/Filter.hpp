#pragma once

#include <memory>
#include "Math.hpp"
#include "Resonix.hpp"

namespace Filter {
    struct BiquadFilter {
        float b0, b1, b2, a1, a2;
        float x1, x2, y1, y2;

        BiquadFilter() {
            reset();
        }

        void reset() {
            b0 = 1.0f;
            b1 = 0.0f;
            b2 = 0.0f;
            a1 = 0.0f;
            a2 = 0.0f;
            x1 = 0.0f;
            x2 = 0.0f;
            y1 = 0.0f;
            y2 = 0.0f;
        }

        void setCoefficients(float b0_, float b1_, float b2_, float a1_, float a2_) {
            b0 = b0_;
            b1 = b1_;
            b2 = b2_;
            a1 = a1_;
            a2 = a2_;
        }

        float process(float input) {
            float output = b0 * input + b1 * x1 + b2 * x2 - a1 * y1 - a2 * y2;

            x2 = x1;
            x1 = input;
            y2 = y1;
            y1 = output;

            return output;
        }
    };

	std::unique_ptr<float[]> apply_bandpass_filter(const float* samples, int sample_length, float center_hz, float bandwidth_hz, float resonance);

    std::unique_ptr<float[]> apply_lowpass_filter(const float* samples, int sample_length, float cutoff_hz, float resonance);

    std::unique_ptr<float[]> apply_highpass_filter(const float* samples, int sample_length, float cutoff_hz, float resonance);

    std::unique_ptr<float[]> apply_formant_filter(const float* samples, int sample_length, float peak, float mix, float spread);
}