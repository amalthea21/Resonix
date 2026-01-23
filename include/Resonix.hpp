#pragma once

#include "Generator.hpp"
#include "Filter.hpp"

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
     * @param frequency Frequency ovoid low_frequency_filter(float* &samples, int sample_length, float cutoff_point);f the waveform in Hz (e.g., 440.0 for A4)
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

    /**
     * @brief Applies a lowpass filter to audio samples using a biquad filter design
     *
     * Filters the input audio samples with a second-order resonant lowpass filter,
     * removing high frequencies above the specified cutoff point while preserving
     * lower frequencies. Uses a digital biquad filter implementation with direct
     * form I structure for numerical stability.
     *
     * @param samples Pointer to input audio samples to filter
     * @param sample_length Number of samples in the input/output buffer
     * @param cutoff_hz Cutoff frequency in Hz (e.g., 1000.0 for 1kHz lowpass)
     * @param resonance Resonance/Q factor of the filter (default: 0.707f for Butterworth response)
     * @return float* Pointer to dynamically allocated array of filtered samples
     *
     * @note Caller is responsible for freeing the returned array with delete[]
     * @warning Returns nullptr if allocation fails, input is invalid, or parameters are out of range
     * @note Higher resonance values (e.g., > 0.9) create a resonant peak near the cutoff frequency
     * @note For stability, resonance should typically be between 0.5 and 10.0
     *
     * @example
     * // Filter audio with 1kHz cutoff and moderate resonance
     * float* filtered = Resonix::lowpass_filter(samples, 44100, 1000.0f, 0.707f);
     * // Use filtered audio...
     * delete[] filtered;
     *
     * @example
     * // Filter with stronger resonance effect (emphasizes cutoff frequency)
     * float* resonant = Resonix::lowpass_filter(samples, 44100, 500.0f, 2.5f);
     * // Has a noticeable "peak" at 500Hz
     * delete[] resonant;
     */
    float* lowpass_filter(const float* samples, int sample_length, float cutoff_hz, float resonance = 0.707f);

    /**
     * @brief Applies a highpass filter to audio samples using a biquad filter design
     *
     * Filters the input audio samples with a second-order resonant highpass filter,
     * removing low frequencies below the specified cutoff point while preserving
     * higher frequencies. Uses a digital biquad filter implementation.
     *
     * @param samples Pointer to input audio samples to filter
     * @param sample_length Number of samples in the input/output buffer
     * @param cutoff_hz Cutoff frequency in Hz (e.g., 200.0 for 200Hz highpass)
     * @param resonance Resonance/Q factor of the filter (default: 0.707f for Butterworth response)
     * @return float* Pointer to dynamically allocated array of filtered samples
     *
     * @note Caller is responsible for freeing the returned array with delete[]
     * @warning Returns nullptr if allocation fails, input is invalid, or parameters are out of range
     * @note Higher resonance values (e.g., > 0.9) create a resonant peak near the cutoff frequency
     * @note For stability, resonance should typically be between 0.5 and 10.0
     *
     * @example
     * // Remove low-frequency rumble below 80Hz
     * float* filtered = Resonix::highpass_filter(samples, 44100, 80.0f, 0.707f);
     * delete[] filtered;
     *
     * @example
     * // Create telephone effect (remove bass)
     * float* phone = Resonix::highpass_filter(samples, 44100, 300.0f, 0.707f);
     * delete[] phone;
     */
    float* highpass_filter(const float* samples, int sample_length, float cutoff_hz, float resonance = 0.707f);
}