#include <iostream>

#include "Resonix.hpp"

int main() {
    constexpr int SAMPLE_LENGTH = 1;
    constexpr float FREQUENCY = 440;

    float* samples = Resonix::generateSamples(Resonix::SAWTOOTH, SAMPLE_LENGTH, FREQUENCY);


    for (int i = 0; i < 1000; i++) {
        std::cout << i << "," << samples[i] << std::endl;
    }

    return 0;
}