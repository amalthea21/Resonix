#include "Resonix.hpp"
#include "Generator.hpp"
#include "Math.hpp"

namespace Generator {
    float* Hann(int sample_length, float frequency, const float phaseIncrement) {
        float* samples = new float[sample_length * Resonix::SAMPLE_RATE];

        for (int i = 0; i < sample_length * Resonix::SAMPLE_RATE; i++) {

        }
    }

    float *Phased_Hann(int sample_length, float frequency, const float phaseIncrement) {
        float* samples = new float[sample_length * Resonix::SAMPLE_RATE];

        for (int i = 0; i < sample_length * Resonix::SAMPLE_RATE; i++) {

        }
    }
}