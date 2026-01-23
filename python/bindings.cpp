#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include "Resonix.hpp"

namespace py = pybind11;

// Wrapper function that returns a NumPy array instead of raw pointer
py::array_t<float> generateSamplesNumPy(Resonix::Shape shape, int sample_length, float frequency) {
    // Generate samples using the C++ function
    float* samples_ptr = Resonix::generateSamples(shape, sample_length, frequency);

    if (!samples_ptr) {
        throw std::runtime_error("Failed to generate samples");
    }

    // Calculate total number of samples
    size_t total_samples = sample_length * Resonix::SAMPLE_RATE;

    // Create a NumPy array that owns the data
    // The capsule will handle deletion when Python is done with the array
    py::capsule free_when_done(samples_ptr, [](void *f) {
        float *foo = reinterpret_cast<float *>(f);
        delete[] foo;
    });

    return py::array_t<float>(
        {total_samples},           // shape
        {sizeof(float)},           // strides
        samples_ptr,               // data pointer
        free_when_done            // capsule for cleanup
    );
}

PYBIND11_MODULE(resonix, m) {
    m.doc() = "Resonix - Audio waveform generation library";

    // Expose the SAMPLE_RATE constant
    m.attr("SAMPLE_RATE") = Resonix::SAMPLE_RATE;

    // Expose the Shape enum
    py::enum_<Resonix::Shape>(m, "Shape", py::arithmetic())
        .value("SINE", Resonix::Shape::SINE, "Sine wave")
        .value("SQUARE", Resonix::Shape::SQUARE, "Square wave")
        .value("TRIANGLE", Resonix::Shape::TRIANGLE, "Triangle wave")
        .value("SAWTOOTH", Resonix::Shape::SAWTOOTH, "Sawtooth wave")
        .value("COSINE", Resonix::Shape::COSINE, "Cosine wave")
        .value("TANGENT", Resonix::Shape::TANGENT, "Tangent wave")
        .value("COTANGENT", Resonix::Shape::COTANGENT, "Cotangent wave")
        .value("HANN", Resonix::Shape::HANN, "Hann window")
        .value("PHASED_HANN", Resonix::Shape::PHASED_HANN, "Phase-shifted Hann window")
        .export_values();

    // Expose the main function
    m.def("generate_samples", &generateSamplesNumPy,
          py::arg("shape"),
          py::arg("sample_length"),
          py::arg("frequency"),
          R"pbdoc(
            Generate audio samples of the specified waveform shape.

            Parameters
            ----------
            shape : Shape
                The waveform shape to generate (e.g., Shape.SINE, Shape.SQUARE)
            sample_length : int
                Number of seconds to generate
            frequency : float
                Frequency of the waveform in Hz (e.g., 440.0 for A4)

            Returns
            -------
            numpy.ndarray
                Array of float32 samples in range [-1.0, 1.0]
                Length will be sample_length * SAMPLE_RATE

            Examples
            --------
            >>> import resonix
            >>> samples = resonix.generate_samples(resonix.Shape.SINE, 1, 440.0)
            >>> print(samples.shape)
            (44100,)
          )pbdoc");
}