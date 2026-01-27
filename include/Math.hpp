#pragma once

/**
 * @namespace Math
 * @brief Mathematical utility functions for audio processing
 *
 * Provides trigonometric and basic mathematical operations optimized for
 * audio synthesis and digital signal processing calculations.
 */
namespace Math {
    constexpr float PI = 3.14159265358979323846f;
    constexpr float TWO_PI = 6.28318530717958647692f;
    constexpr float DEG_TO_RAD = PI / 180.0f;
    constexpr float RAD_TO_DEG = 180.0f / PI;
    constexpr float INV_360 = 1.0f / 360.0f;
    constexpr float EPSILON = 1e-6f;

    /**
     * @brief Computes the sine of an angle
     *
     * @param degrees Angle in degrees
     * @return float Sine value in range [-1.0, 1.0]
     *
     * @note Input is in degrees, not radians
     * @see Cosine(), Tangent()
     */
    float Sine(float degrees);


    /**
     * @brief Clamps a value between minimum and maximum bounds
     *
     * @param value The input value to clamp
     * @param min Lower bound (inclusive)
     * @param max Upper bound (inclusive)
     * @return float Clamped value in range [min, max]
     *
     * @note Returns min if value < min, max if value > max, otherwise returns value unchanged
     * @note Ensure min <= max for correct behavior. If min > max, behavior is undefined.
     * @see Sine(), Cosine(), Tangent()
     */
    float clamp(float value, float min, float max);

    /**
     * @brief Computes the cosine of an angle
     *
     * @param degrees Angle in degrees
     * @return float Cosine value in range [-1.0, 1.0]
     *
     * @note Input is in degrees, not radians
     * @see Sine(), Tangent()
     */
    float Cosine(float degrees);

    /**
     * @brief Computes the tangent of an angle
     *
     * @param degrees Angle in degrees
     * @return float Tangent value
     *
     * @note Input is in degrees, not radians
     * @warning Returns undefined/very large values near 90° + n*180°
     * @see Sine(), Cosine(), Cotangent()
     */
    float Tangent(float degrees);

    /**
     * @brief Computes the cotangent of an angle
     *
     * @param degrees Angle in degrees
     * @return float Cotangent value (1/tan)
     *
     * @note Input is in degrees, not radians
     * @warning Returns undefined/very large values near n*180°
     * @see Tangent()
     */
    float Cotangent(float degrees);

    /**
     * @brief Computes the Hann window function value
     *
     * Calculates the Hann (raised cosine) window function, commonly used
     * in audio windowing to reduce spectral leakage and smooth signal edges.
     * The function produces a smooth bell-shaped curve useful for envelope
     * generation and FFT analysis.
     *
     * @param degrees Position in the window (0-360 degrees represents full window)
     * @return float Window coefficient value in range [0.0, 1.0]
     *
     * @note Input is in degrees, not radians
     * @note Returns 0.0 at 0° and 360°, and 1.0 at 180°
     *
     * @example
     * // Generate Hann window envelope
     * float envelope = Math::Hann(phase); // phase from 0 to 360
     *
     * @see https://en.wikipedia.org/wiki/Hann_function
     */
    float Hann(float n, float N);

    /**
     * @brief Computes the floating-point remainder of division
     *
     * Calculates the remainder of x/y with the same sign as x.
     * Useful for phase wrapping in oscillators.
     *
     * @param x Dividend
     * @param y Divisor
     * @return float Remainder of x/y
     *
     * @warning Returns undefined if y is zero
     *
     * @example
     * float wrapped = Math::fmod(phase, 360.0f); // Wrap phase to [0, 360)
     */
    float fmod(float x, float y);

    /**
     * @brief Returns the minimum of two values
     *
     * @param a First value
     * @param b Second value
     * @return float The smaller of a and b
     *
     * @note If values are equal, returns a
     */
    float min(float a, float b);

    /**
     * @brief Computes the absolute value
     *
     * @param a Input value
     * @return float Non-negative absolute value of a
     *
     * @example
     * float distance = Math::abs(value1 - value2);
     */
    float abs(float a);

    /**
     * @brief Returns a Not-a-Number (NaN) floating-point value
     *
     * Generates a special IEEE 754 floating-point value that represents
     * an undefined or unrepresentable result. Useful for signaling invalid
     * states or uninitialized values in audio processing.
     *
     * @return float IEEE 754 NaN value
     *
     * @note NaN has the property that NaN != NaN evaluates to true
     * @note Use isNaN() to safely check for NaN values
     *
     * @example
     * float result = Math::getNaN(); // Initialize with invalid state
     * if (Math::isNaN(result)) {
     *     // Handle uninitialized value
     * }
     *
     * @see isNaN()
     */
    float getNaN();

    /**
     * @brief Checks if a floating-point value is Not-a-Number (NaN)
     *
     * Tests whether the input value is a NaN (Not-a-Number) according to
     * IEEE 754 standard. This is the safe way to check for NaN values,
     * since direct comparison (a == NaN) will always return false.
     *
     * @param a Value to test
     * @return bool true if a is NaN, false otherwise
     *
     * @note This is necessary because NaN != NaN by IEEE 754 definition
     * @note Useful for validating computation results in DSP chains
     *
     * @example
     * float result = computeValue();
     * if (Math::isNaN(result)) {
     *     result = 0.0f; // Fallback to safe default
     * }
     *
     * @see getNaN()
     */
    bool isNaN(float a);
}