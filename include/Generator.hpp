#include "Math.hpp"

/**
 * @namespace Generator
 * @brief Low-level waveform generation functions
 *
 * Provides direct access to individual waveform generators with explicit
 * phase control. These functions generate raw audio sample buffers for
 * various waveform types used in synthesis and signal processing.
 *
 * All generators use phase accumulation for accurate frequency generation
 * and return dynamically allocated buffers that must be freed by the caller.
 */
namespace Generator {
    /**
     * @brief Generates a sine wave
     *
     * Creates a buffer of sine wave samples using phase accumulation for
     * precise frequency control. Produces a pure tone with no harmonics.
     *
     * @param sample_length Number of samples to generate
     * @param frequency Frequency of the waveform in Hz
     * @param phaseIncrement Phase increment per sample (2π * frequency / sample_rate)
     * @return float* Pointer to dynamically allocated array of samples in range [-1.0, 1.0]
     *
     * @note Caller is responsible for freeing the returned array with delete[]
     * @warning Returns nullptr if allocation fails
     *
     * @see Cosine(), Square(), Triangle()
     */
    float* Sine(int sample_length, float frequency, const float phaseIncrement);

    /**
     * @brief Generates a square wave
     *
     * Creates a buffer of square wave samples alternating between -1.0 and 1.0.
     * Contains only odd harmonics, producing a hollow, clarinet-like timbre.
     *
     * @param sample_length Number of samples to generate
     * @param frequency Frequency of the waveform in Hz
     * @param phaseIncrement Phase increment per sample (2π * frequency / sample_rate)
     * @return float* Pointer to dynamically allocated array of samples (values: -1.0 or 1.0)
     *
     * @note Caller is responsible for freeing the returned array with delete[]
     * @warning Returns nullptr if allocation fails
     *
     * @see Triangle(), Sawtooth()
     */
    float* Square(int sample_length, float frequency, const float phaseIncrement);

    /**
     * @brief Generates a triangle wave
     *
     * Creates a buffer of triangle wave samples with linear rise and fall.
     * Contains only odd harmonics with rapid roll-off, producing a mellow tone.
     *
     * @param sample_length Number of samples to generate
     * @param frequency Frequency of the waveform in Hz
     * @param phaseIncrement Phase increment per sample (2π * frequency / sample_rate)
     * @return float* Pointer to dynamically allocated array of samples in range [-1.0, 1.0]
     *
     * @note Caller is responsible for freeing the returned array with delete[]
     * @warning Returns nullptr if allocation fails
     *
     * @see Square(), Sawtooth()
     */
    float* Triangle(int sample_length, float frequency, const float phaseIncrement);

    /**
     * @brief Generates a sawtooth wave
     *
     * Creates a buffer of sawtooth wave samples with linear rise and sharp fall.
     * Contains both odd and even harmonics, producing a bright, buzzy timbre.
     *
     * @param sample_length Number of samples to generate
     * @param frequency Frequency of the waveform in Hz
     * @param phaseIncrement Phase increment per sample (2π * frequency / sample_rate)
     * @return float* Pointer to dynamically allocated array of samples in range [-1.0, 1.0]
     *
     * @note Caller is responsible for freeing the returned array with delete[]
     * @warning Returns nullptr if allocation fails
     *
     * @see Triangle(), Square()
     */
    float* Sawtooth(int sample_length, float frequency, const float phaseIncrement);

    /**
     * @brief Generates a cosine wave
     *
     * Creates a buffer of cosine wave samples. Identical to sine wave but
     * phase-shifted by 90 degrees. Produces a pure tone with no harmonics.
     *
     * @param sample_length Number of samples to generate
     * @param frequency Frequency of the waveform in Hz
     * @param phaseIncrement Phase increment per sample (2π * frequency / sample_rate)
     * @return float* Pointer to dynamically allocated array of samples in range [-1.0, 1.0]
     *
     * @note Caller is responsible for freeing the returned array with delete[]
     * @warning Returns nullptr if allocation fails
     *
     * @see Sine(), Tangent()
     */
    float* Cosine(int sample_length, float frequency, const float phaseIncrement);

    /**
     * @brief Generates a tangent wave
     *
     * Creates a buffer of tangent wave samples with periodic discontinuities.
     * Produces extreme harmonic content with characteristic periodic "spikes".
     *
     * @param sample_length Number of samples to generate
     * @param frequency Frequency of the waveform in Hz
     * @param phaseIncrement Phase increment per sample (2π * frequency / sample_rate)
     * @return float* Pointer to dynamically allocated array of samples
     *
     * @note Caller is responsible for freeing the returned array with delete[]
     * @warning Contains discontinuities; output is not band-limited
     * @warning Returns nullptr if allocation fails
     *
     * @see Cotangent(), Sine()
     */
    float* Tangent(int sample_length, float frequency, const float phaseIncrement);

    /**
     * @brief Generates a cotangent wave
     *
     * Creates a buffer of cotangent wave samples (reciprocal of tangent).
     * Produces extreme harmonic content with periodic discontinuities.
     *
     * @param sample_length Number of samples to generate
     * @param frequency Frequency of the waveform in Hz
     * @param phaseIncrement Phase increment per sample (2π * frequency / sample_rate)
     * @return float* Pointer to dynamically allocated array of samples
     *
     * @note Caller is responsible for freeing the returned array with delete[]
     * @warning Contains discontinuities; output is not band-limited
     * @warning Returns nullptr if allocation fails
     *
     * @see Tangent(), Cosine()
     */
    float* Cotangent(int sample_length, float frequency, const float phaseIncrement);

    /**
     * @brief Generates a Hann window function
     *
     * Creates a buffer using the Hann (raised cosine) window function.
     * Produces a smooth bell-shaped envelope commonly used in spectral analysis
     * and as an amplitude envelope in synthesis.
     *
     * @param sample_length Number of samples to generate
     * @param frequency Frequency of the window cycle in Hz
     * @param phaseIncrement Phase increment per sample (2π * frequency / sample_rate)
     * @return float* Pointer to dynamically allocated array of samples in range [0.0, 1.0]
     *
     * @note Caller is responsible for freeing the returned array with delete[]
     * @note Output range is [0.0, 1.0] unlike other generators
     * @warning Returns nullptr if allocation fails
     *
     * @see Phased_Hann()
     */
    float* Hann(int sample_length, float frequency, const float phaseIncrement);

    /**
     * @brief Generates a phase-shifted Hann window function
     *
     * Creates a buffer using a phase-shifted Hann window function.
     * Similar to standard Hann but with different phase characteristics,
     * useful for creating variations in envelope shapes.
     *
     * @param sample_length Number of samples to generate
     * @param frequency Frequency of the window cycle in Hz
     * @param phaseIncrement Phase increment per sample (2π * frequency / sample_rate)
     * @return float* Pointer to dynamically allocated array of samples in range [0.0, 1.0]
     *
     * @note Caller is responsible for freeing the returned array with delete[]
     * @note Output range is [0.0, 1.0] unlike other generators
     * @warning Returns nullptr if allocation fails
     *
     * @see Hann()
     */
    float* Phased_Hann(int sample_length, float frequency, const float phaseIncrement);
}