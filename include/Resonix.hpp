/**
* @namespace Resonix
 * @brief Audio sample generation library
 */
namespace Resonix {
#ifndef RESONIX_SAMPLE_RATE
    /** @brief Default sample rate in Hz (44.1 kHz) */
    constexpr int SAMPLE_RATE = 44100;
#else
    /** @brief User-defined sample rate in Hz */
    constexpr int SAMPLE_RATE = RESONIX_SAMPLE_RATE;
#endif

    /**
     * @enum Shape
     * @brief Waveform shape types for generation
     */
    enum Shape {
        // Primitive waveforms
        SINE,       ///< Sine wave
        SQUARE,     ///< Square wave
        TRIANGLE,   ///< Triangle wave
        SAWTOOTH,   ///< Sawtooth wave

        // Trigonometric waveforms
        COSINE,     ///< Cosine wave
        TANGENT,    ///< Tangent wave
        COTANGENT,  ///< Cotangent wave

        // Hann functions
        HANN,        ///< Hann window
        PHASED_HANN  ///< Phase-shifted Hann window
    };

    /**
     * @brief Generates audio samples of the specified waveform shape
     *
     * Generates a buffer of floating-point audio samples for the given waveform
     * type at the specified frequency and length. Uses the global SAMPLE_RATE
     * constant to determine the sampling frequency.
     *
     * @param shape The waveform shape to generate (e.g., SINE, SQUARE, TRIANGLE)
     * @param sample_length Number of samples in seconds to generate in the output buffer
     * @param frequency Frequency of the waveform in Hz (e.g., 440.0 for A4)
     * @return float* Pointer to dynamically allocated array of samples in range [-1.0, 1.0]
     *
     * @note Caller is responsible for freeing the returned array with delete[]
     * @warning Returns nullptr if allocation fails or if parameters are invalid
     *
     * @example
     * // Generate 1 second of 440Hz sine wave at 44100 Hz sample rate
     * float* samples = Resonix::generateSamples(Resonix::SINE, 44100, 440.0f);
     * // Use samples...
     * delete[] samples;
     */
    float* generateSamples(Shape shape, int sample_length, float frequency);
}