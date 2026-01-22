#include "Resonix.hpp"
#include "Generator.hpp"
#include "Math.hpp"

namespace Generator {
    float *Sine(int sample_length, float frequency, const float phaseIncrement) {
        float* samples = new float[sample_length * Resonix::SAMPLE_RATE];
        float t;

        for (int i = 0; i < sample_length * Resonix::SAMPLE_RATE; i++) {
            t = static_cast<float>(i) / Resonix::SAMPLE_RATE;
            samples[i] = Math::Sine(2.0f * Math::PI * frequency * t * 180.0f / Math::PI);
        }
    }

    float *Square(int sample_length, float frequency, const float phaseIncrement) {
        float* samples = new float[sample_length * Resonix::SAMPLE_RATE];
        float t, phase;

        for (int i = 0; i < sample_length * Resonix::SAMPLE_RATE; i++) {
            t = static_cast<float>(i) / Resonix::SAMPLE_RATE;
            phase = Math::fmod(t * frequency, 1.0f);
            samples[i] = phase < 0.5f ? 1.0f : -1.0f;
        }
    }

    float *Triangle(int sample_length, float frequency, const float phaseIncrement) {
        float* samples = new float[sample_length * Resonix::SAMPLE_RATE];
        float t, phase;

        for (int i = 0; i < sample_length * Resonix::SAMPLE_RATE; i++) {
            t = static_cast<float>(i) / Resonix::SAMPLE_RATE;
            phase = Math::fmod(t * frequency, 1.0f);
            samples[i] = phase < 0.5f ? 4.0f * phase - 1.0f: 3.0f - 4.0f * phase;
        }
    }

    float *Sawtooth(int sample_length, float frequency, const float phaseIncrement) {
        float* samples = new float[sample_length * Resonix::SAMPLE_RATE];
        float t, phase;

        for (int i = 0; i < sample_length * Resonix::SAMPLE_RATE; i++) {
            t = static_cast<float>(i) / Resonix::SAMPLE_RATE;
            phase = Math::fmod(t * frequency, 1.0f);
            samples[i] = 2.0f * phase - 1.0f;
        }
    }
}