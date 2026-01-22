/**
 * @namespace Math
 * @brief Mathematical utility functions for audio processing
 *
 * Provides trigonometric and basic mathematical operations optimized for
 * audio synthesis and digital signal processing calculations.
 */
namespace Math {
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

    float getNaN();

    bool isNaN(float a);
}