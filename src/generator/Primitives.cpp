#include "Resonix.hpp"
#include "Generator.hpp"
#include "Math.hpp"

namespace Generator {
    std::unique_ptr<float[]> Sine(int sample_length, float frequency, const float phaseIncrement) {
        std::unique_ptr<float[]> samples = new float[sample_length * Resonix::SAMPLE_RATE];
        float t, phase;

        for (int i = 0; i < sample_length * Resonix::SAMPLE_RATE; i++) {
            t = static_cast<float>(i) / Resonix::SAMPLE_RATE;
            phase = 2.0f * Math::PI * frequency * t + phaseIncrement;
            samples[i] = Math::Sine(phase * 180.0f / Math::PI);
        }

        return samples;
    }

    std::unique_ptr<float[]> Square(int sample_length, float frequency, const float phaseIncrement) {
        std::unique_ptr<float[]> samples = new float[sample_length * Resonix::SAMPLE_RATE];
        float t, phase;

        for (int i = 0; i < sample_length * Resonix::SAMPLE_RATE; i++) {
            t = static_cast<float>(i) / Resonix::SAMPLE_RATE;
            phase = Math::fmod(t * frequency + phaseIncrement / (2.0f * Math::PI), 1.0f);
            samples[i] = phase < 0.5f ? 1.0f : -1.0f;
        }

        return samples;
    }

    std::unique_ptr<float[]> Triangle(int sample_length, float frequency, const float phaseIncrement) {
        std::unique_ptr<float[]> samples = new float[sample_length * Resonix::SAMPLE_RATE];
        float t, phase;

        for (int i = 0; i < sample_length * Resonix::SAMPLE_RATE; i++) {
            t = static_cast<float>(i) / Resonix::SAMPLE_RATE;
            phase = Math::fmod(t * frequency + phaseIncrement / (2.0f * Math::PI), 1.0f);
            samples[i] = phase < 0.5f ? 4.0f * phase - 1.0f : 3.0f - 4.0f * phase;
        }

        return samples;
    }

    std::unique_ptr<float[]> Sawtooth(int sample_length, float frequency, const float phaseIncrement) {
        std::unique_ptr<float[]> samples = new float[sample_length * Resonix::SAMPLE_RATE];
        float t, phase;

        for (int i = 0; i < sample_length * Resonix::SAMPLE_RATE; i++) {
            t = static_cast<float>(i) / Resonix::SAMPLE_RATE;
            phase = Math::fmod(t * frequency + phaseIncrement / (2.0f * Math::PI), 1.0f);
            samples[i] = 2.0f * phase - 1.0f;
        }

        return samples;
    }
}