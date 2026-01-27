#include "Resonix.hpp"

namespace Resonix {
    std::unique_ptr<float[]> generateSamples(Shape shape, int sample_length, float frequency) {
        if (sample_length <= 0 || frequency <= 0.0f)
            return nullptr;

        const float phase_increment = (2.0f * 3.14159265359f * frequency) / SAMPLE_RATE;

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

    std::unique_ptr<float[]> lowpass_filter(const float* samples, int sample_length, float cutoff_hz, float resonance) {
        return Filter::apply_lowpass_filter(samples, sample_length, cutoff_hz, resonance);
    }

    std::unique_ptr<float[]> highpass_filter(const float* samples, int sample_length, float cutoff_hz, float resonance) {
        return Filter::apply_highpass_filter(samples, sample_length, cutoff_hz, resonance);
    }

    std::unique_ptr<float[]> formant_filter(const float* samples, int sample_length, float peak, float mix, float spread) {
        return Filter::apply_formant_filter(samples, sample_length, peak, mix, spread);
    }

	std::unique_ptr<float[]> bandpass_filter(const float* samples, int sample_length, float center_hz, float bandwidth_hz, float resonance) {
		return Filter::apply_bandpass_filter(samples, sample_length, center_hz, bandwidth_hz, resonance);
	}
}
