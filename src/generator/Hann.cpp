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
            sine_wave = Math::Sine(2.0f * Math::PI * frequency * t);
            hann_window = Math::Hann(static_cast<float>(i), N);
            samples[i] = sine_wave * hann_window;
        }

        return samples;
    }

    float *Phased_Hann(int sample_length, float frequency, const float phaseIncrement) {
        float* samples = new float[sample_length * Resonix::SAMPLE_RATE];
        int totalSamples = sample_length * Resonix::SAMPLE_RATE;

        float samplesPerCycle = Resonix::SAMPLE_RATE / frequency;

        for (int i = 0; i < totalSamples; i++) {
            float phaseOffset = phaseIncrement / (2.0f * Math::PI);
            float position = Math::fmod(i / samplesPerCycle + phaseOffset, 1.0f);
            float n = position * samplesPerCycle;

            samples[i] = Math::Hann(n, samplesPerCycle);
        }

        return samples;
    }
}