namespace Generator {
    float* Sine(int sample_length, float frequency, const float phaseIncrement);
    float* Square(int sample_length, float frequency, const float phaseIncrement);
    float* Triangle(int sample_length, float frequency, const float phaseIncrement);
    float* Sawtooth(int sample_length, float frequency, const float phaseIncrement);

    float* Cosine(int sample_length, float frequency, const float phaseIncrement);
    float* Tangent(int sample_length, float frequency, const float phaseIncrement);
    float* Cotangent(int sample_length, float frequency, const float phaseIncrement);

    float* Hann(int sample_length, float frequency, const float phaseIncrement);
    float* Phased_Hann(int sample_length, float frequency, const float phaseIncrement);
}