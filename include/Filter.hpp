#include "Math.hpp"
#include "Resonix.hpp"

namespace Filter {
    float* apply_lowpass_filter(const float* samples, int sample_length, float cutoff_hz, float resonance);
}