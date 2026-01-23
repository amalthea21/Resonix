#include "Resonix.hpp"
#include "Generator.hpp"
#include "Math.hpp"

namespace Generator {
    float* Hann(int sample_length, float frequency, const float phaseIncrement) {
        float* samples = new float[static_cast<int>(sample_length * Resonix::SAMPLE_RATE)];
        float t, sine_wave, hann_window;
        int N = static_cast<int>(sample_length * Resonix::SAMPLE_RATE);

        for (int i = 0; i < sample_length * Resonix::SAMPLE_RATE; i++) {
            t = static_cast<float>(i) / Resonix::SAMPLE_RATE;
            sine_wave = Math::Sine(360.0f * frequency * t);
            hann_window = Math::Hann(static_cast<float>(i), N);
            samples[i] = sine_wave * hann_window;
        }

        return samples;
    }

    float *Phased_Hann(int sample_length, float frequency, const float phaseIncrement) {
        float* samples = new float[sample_length * Resonix::SAMPLE_RATE];
        float t, sine_wave, hann_window;
        int N = static_cast<int>(sample_length * Resonix::SAMPLE_RATE);
        float phaseOffsetSamples = (phaseIncrement / (2.0f * Math::PI)) * N;

        for (int i = 0; i < sample_length * Resonix::SAMPLE_RATE; i++) {
            t = static_cast<float>(i) / Resonix::SAMPLE_RATE;
            sine_wave = Math::Sine(360.0f * frequency * t + phaseIncrement * 180.0f / Math::PI);
            float windowPosition = Math::fmod(static_cast<float>(i) + phaseOffsetSamples, static_cast<float>(N));
            if (windowPosition < 0.0f) windowPosition += N;

            hann_window = Math::Hann(windowPosition, N);
            samples[i] = sine_wave * hann_window;
        }

        return samples;
    }
}