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
        float t, phase;
        int totalSamples = sample_length * Resonix::SAMPLE_RATE;

        for (int i = 0; i < totalSamples; i++) {
            t = static_cast<float>(i) / Resonix::SAMPLE_RATE;
            phase = Math::fmod(t * frequency + phaseIncrement / (2.0f * Math::PI), 1.0f);
            float n = phase * totalSamples;
            samples[i] = Math::Hann(n, static_cast<float>(totalSamples));
        }

        return samples;
    }
}