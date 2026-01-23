#include "Resonix.hpp"
#include "Generator.hpp"
#include "Math.hpp"

namespace Generator {
    float* Hann(int sample_length, float frequency, const float phaseIncrement) {
        float* samples = new float[sample_length * Resonix::SAMPLE_RATE];
        float t, phase;
        int N = sample_length * Resonix::SAMPLE_RATE;

        for (int i = 0; i < sample_length * Resonix::SAMPLE_RATE; i++) {
            samples[i] = Math::Hann(static_cast<float>(i), static_cast<float>(N));
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