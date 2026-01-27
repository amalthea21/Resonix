#pragma once

#include <memory>
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
     * @param frequency Frequency of the waveform in Hz (e.g., 440.0 for A4)
     * @return std::unique_ptr<float[]> Pointer to dynamically allocated array of samples in range [-1.0, 1.0]
     *
     * @note Caller is responsible for freeing the returned array with delete[]
     * @warning Returns nullptr if allocation fails or if parameters are invalid
     *
     * @example
     * // Generate 1 second of 440Hz sine wave at 44100 Hz sample rate
     * std::unique_ptr<float[]> samples = Resonix::generateSamples(Resonix::SINE, 44100, 440.0f);
     * // Use samples...
     * delete[] samples;
     */
    std::unique_ptr<float[]> generateSamples(Shape shape, int sample_length, float frequency);

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
     * @return std::unique_ptr<float[]> Pointer to dynamically allocated array of filtered samples
     *
     * @note Caller is responsible for freeing the returned array with delete[]
     * @warning Returns nullptr if allocation fails, input is invalid, or parameters are out of range
     * @note Higher resonance values (e.g., > 0.9) create a resonant peak near the cutoff frequency
     * @note For stability, resonance should typically be between 0.5 and 10.0
     *
     * @example
     * // Filter audio with 1kHz cutoff and moderate resonance
     * std::unique_ptr<float[]> filtered = Resonix::lowpass_filter(samples, 44100, 1000.0f, 0.707f);
     * // Use filtered audio...
     * delete[] filtered;
     *
     * @example
     * // Filter with stronger resonance effect (emphasizes cutoff frequency)
     * std::unique_ptr<float[]> resonant = Resonix::lowpass_filter(samples, 44100, 500.0f, 2.5f);
     * // Has a noticeable "peak" at 500Hz
     * delete[] resonant;
     */
    std::unique_ptr<float[]> lowpass_filter(const float* samples, int sample_length, float cutoff_hz, float resonance = 0.707f);

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
     * @return std::unique_ptr<float[]> Pointer to dynamically allocated array of filtered samples
     *
     * @note Caller is responsible for freeing the returned array with delete[]
     * @warning Returns nullptr if allocation fails, input is invalid, or parameters are out of range
     * @note Higher resonance values (e.g., > 0.9) create a resonant peak near the cutoff frequency
     * @note For stability, resonance should typically be between 0.5 and 10.0
     *
     * @example
     * // Remove low-frequency rumble below 80Hz
     * std::unique_ptr<float[]> filtered = Resonix::highpass_filter(samples, 44100, 80.0f, 0.707f);
     * delete[] filtered;
     *
     * @example
     * // Create telephone effect (remove bass)
     * std::unique_ptr<float[]> phone = Resonix::highpass_filter(samples, 44100, 300.0f, 0.707f);
     * delete[] phone;
     */
    std::unique_ptr<float[]> highpass_filter(const float* samples, int sample_length, float cutoff_hz, float resonance = 0.707f);

    /**
     * @brief Applies a bandpass filter to audio samples using a biquad filter design
     *
     * Filters the input audio samples to isolate a specific frequency band by removing
     * frequencies both above and below the specified range. Uses a second-order resonant
     * bandpass filter implementation based on biquad coefficients. This is particularly
     * useful for consonant synthesis, isolating instrument harmonics, or creating
     * telephone/radio effects.
     *
     * @param samples Pointer to input audio samples to filter
     * @param sample_length Number of samples in the input/output buffer
     * @param center_hz Center frequency of the passband in Hz (e.g., 1000.0 for 1kHz center)
     * @param bandwidth_hz Width of the passband in Hz (e.g., 200.0 for ±100Hz around center)
     * @param resonance Resonance/Q multiplier of the filter (default: 0.707f for moderate response)
     * @return std::unique_ptr<float[]> Pointer to dynamically allocated array of filtered samples
     *
     * @note Caller is responsible for freeing the returned array with delete[]
     * @note The filter passes frequencies approximately from (center_hz - bandwidth_hz/2) to (center_hz + bandwidth_hz/2)
     * @note Actual Q factor is calculated as: Q = (center_hz / bandwidth_hz) * resonance
     * @note Narrower bandwidths create sharper, more selective filtering
     * @warning Returns nullptr if allocation fails, input is invalid, or parameters are out of range
     * @warning Very narrow bandwidths (high Q) may cause ringing artifacts
     * @warning Center frequency should be well within Nyquist limit (SAMPLE_RATE / 2)
     *
     * @example
     * // Isolate frequencies around 1kHz with 500Hz bandwidth (750Hz - 1250Hz)
     * std::unique_ptr<float[]> filtered = Resonix::bandpass_filter(samples, 44100, 1000.0f, 500.0f);
     * delete[] filtered;
     *
     * @example
     * // Create narrow bandpass for fricative 's' sound (4kHz - 8kHz range)
     * std::unique_ptr<float[]> fricative = Resonix::bandpass_filter(noise, 44100, 6000.0f, 4000.0f, 0.5f);
     * delete[] fricative;
     *
     * @example
     * // Telephone effect - isolate voice frequencies (300Hz - 3400Hz)
     * std::unique_ptr<float[]> phone = Resonix::bandpass_filter(voice, 44100, 1850.0f, 3100.0f, 0.707f);
     * delete[] phone;
     *
     * @see lowpass_filter(), highpass_filter(), formant_filter()
     */
    std::unique_ptr<float[]> bandpass_filter(const float* samples, int sample_length, float center_hz, float bandwidth_hz, float resonance = 0.707f);

    /**
     * @brief Applies a formant filter to simulate vowel sounds and vocal characteristics
     *
     * Applies a series of resonant bandpass filters to simulate the formant structure
     * of human vowels. Formants are resonant frequencies of the vocal tract that give
     * vowels their characteristic timbre. This filter creates vocal-like or vowel-like
     * timbres from any input audio, making it useful for speech synthesis, vocal effects,
     * and talkbox-style processing.
     *
     * The filter implements 4 formant bands with frequency-dependent Q values and weights,
     * modeling the first four formants (F1-F4) that are critical for vowel perception.
     * Different peak values correspond to different vowel sounds based on formant positioning.
     *
     * @param samples Pointer to input audio samples to filter
     * @param sample_length Number of samples in the input/output buffer
     * @param peak Vowel formant selection parameter (0.0 - 1.0), determines which vowel to simulate:
     *             - 0.0 - 0.2: "ah" as in "father" (F1=800Hz, F2=1150Hz)
     *             - 0.2 - 0.4: "eh" as in "bed" (F1=400Hz, F2=1600Hz)
     *             - 0.4 - 0.6: "ee" as in "see" (F1=350Hz, F2=1700Hz)
     *             - 0.6 - 0.8: "oh" as in "go" (F1=450Hz, F2=800Hz)
     *             - 0.8 - 1.0: "oo" as in "boot" (F1=325Hz, F2=700Hz)
     * @param mix Dry/wet mix ratio (0.0 - 1.0):
     *            - 0.0 = completely dry (original signal)
     *            - 0.5 = 50% dry, 50% wet (balanced)
     *            - 1.0 = completely wet (only filtered signal)
     * @param spread Formant frequency spread/width (0.0 - 1.0):
     *               - 0.0 = normal formant spacing
     *               - 0.5 = moderately widened formants
     *               - 1.0 = widely spread formants (creates more diffuse vocal character)
     * @return std::unique_ptr<float[]> Pointer to dynamically allocated array of filtered samples
     *
     * @note Caller is responsible for freeing the returned array with delete[]
     * @note Works best with harmonically rich input signals (sawtooth, square waves)
     * @note All parameters are automatically clamped to valid ranges [0.0, 1.0]
     * @note The filter uses high Q values (8-20) to create sharp formant peaks
     * @warning Returns nullptr if allocation fails or input is invalid
     * @warning May introduce ringing on transient signals due to high resonance
     *
     * @example
     * // Create "ee" vowel sound from sawtooth wave
     * auto saw = Resonix::generateSamples(Resonix::SAWTOOTH, 1, 110.0f);
     * std::unique_ptr<float[]> vowel_ee = Resonix::formant_filter(saw.get(), 44100, 0.5f, 1.0f, 0.0f);
     * delete[] vowel_ee;
     *
     * @example
     * // Subtle vocal character with 50% mix
     * std::unique_ptr<float[]> subtle = Resonix::formant_filter(audio, 44100, 0.3f, 0.5f, 0.0f);
     * delete[] subtle;
     *
     * @example
     * // Wide, diffuse "ah" vowel
     * std::unique_ptr<float[]> wide_ah = Resonix::formant_filter(synth, 44100, 0.1f, 0.8f, 0.7f);
     * delete[] wide_ah;
     *
     * @example
     * // Talkbox-style effect morphing between vowels
     * for (float peak = 0.0f; peak <= 1.0f; peak += 0.01f) {
     *     auto vowel = Resonix::formant_filter(carrier, length, peak, 1.0f, 0.2f);
     *     // Process morphing vowel sound...
     * }
     *
     * @see bandpass_filter() for individual formant band simulation
     * @see https://en.wikipedia.org/wiki/Formant for more information on formants
     */
    std::unique_ptr<float[]> formant_filter(const float* samples, int sample_length, float peak, float mix, float spread);
}