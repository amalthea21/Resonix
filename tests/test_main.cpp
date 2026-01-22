#include <iostream>

#include "Resonix.hpp"

int main() {
    float* samples = Resonix::generateSamples(Resonix::Shape::SINE, 1, 440.0f);

    for (int i = 0; i < 1*44100; i++) {
        std::cout << i << "," << samples[i] << std::endl;
    }

    return 0;
}