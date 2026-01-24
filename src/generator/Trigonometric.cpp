#include "Resonix.hpp"
#include "Generator.hpp"
#include "Math.hpp"

namespace Generator {
    std::unique_ptr<float[]> Cosine(int sample_length, float frequency, const float phaseIncrement) {
        std::unique_ptr<float[]> samples = std::make_unique<float[]>(sample_length * Resonix::SAMPLE_RATE);
        float t, phase;

        for (int i = 0; i < sample_length * Resonix::SAMPLE_RATE; i++) {
            t = static_cast<float>(i) / Resonix::SAMPLE_RATE;
            phase = 2.0f * Math::PI * frequency * t + phaseIncrement;
            samples[i] = Math::Cosine(phase * 180.0f / Math::PI);
        }

        return samples;
    }

    std::unique_ptr<float[]> Tangent(int sample_length, float frequency, const float phaseIncrement) {
        std::unique_ptr<float[]> samples = std::make_unique<float[]>(sample_length * Resonix::SAMPLE_RATE);
        float t, phase;

        for (int i = 0; i < sample_length * Resonix::SAMPLE_RATE; i++) {
            t = static_cast<float>(i) / Resonix::SAMPLE_RATE;
            phase = 2.0f * Math::PI * frequency * t + phaseIncrement;
            samples[i] = Math::Tangent(phase * 180.0f / Math::PI);
        }

        return samples;
    }

    std::unique_ptr<float[]> Cotangent(int sample_length, float frequency, const float phaseIncrement) {
        std::unique_ptr<float[]> samples = std::make_unique<float[]>(sample_length * Resonix::SAMPLE_RATE);
        float t, phase;

        for (int i = 0; i < sample_length * Resonix::SAMPLE_RATE; i++) {
            t = static_cast<float>(i) / Resonix::SAMPLE_RATE;
            phase = 2.0f * Math::PI * frequency * t + phaseIncrement;
            samples[i] = Math::Cotangent(phase * 180.0f / Math::PI);
        }

        return samples;
    }
}