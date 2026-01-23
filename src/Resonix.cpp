#include "../include/Resonix.hpp"

namespace Resonix {
    float *generateSamples(Shape shape, int sample_length, float frequency) {
        if (sample_length <= 0 || frequency <= 0.0f)
            return nullptr;

        float* samples = new float[sample_length * SAMPLE_RATE];
        const float phase_increment = (2.0f * 3.14159265359f * frequency) / SAMPLE_RATE;

        if (!samples) return nullptr;

        switch (shape) {
            case SINE:
                return Generator::Sine(sample_length, frequency, phase_increment);
            case SQUARE:
                return Generator::Square(sample_length, frequency, phase_increment);
            case TRIANGLE:
                return Generator::Triangle(sample_length, frequency, phase_increment);
            case SAWTOOTH:
                return Generator::Sawtooth(sample_length, frequency, phase_increment);
            case COSINE:
                return Generator::Cosine(sample_length, frequency, phase_increment);
            case TANGENT:
                return Generator::Tangent(sample_length, frequency, phase_increment);
            case COTANGENT:
                return Generator::Cotangent(sample_length, frequency, phase_increment);
            case HANN:
                return Generator::Hann(sample_length, frequency, phase_increment);
            case PHASED_HANN:
                return Generator::Phased_Hann(sample_length, frequency, phase_increment);
            default:
                return nullptr;
        }
    }
}
