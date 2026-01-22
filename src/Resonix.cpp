#include "../include/Resonix.hpp"

namespace Resonix {
    float *generateSamples(Shape shape, int sample_length, float frequency) {
        if (sample_length <= 0 || sample_length <= 0.0f)
            return nullptr;

        float* samples = new float[sample_length * SAMPLE_RATE];
        const float phase_increment = (2.0f * 3.14159265359f * frequency) / SAMPLE_RATE;

        switch (shape) {

        }
    }
}
