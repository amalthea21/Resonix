#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <stdexcept>
#include "Resonix.hpp"

namespace py = pybind11;

py::array_t<float> generateSamplesNumPy(Resonix::Shape shape, int sample_length, float frequency) {
    if (sample_length <= 0) {
        throw std::invalid_argument("sample_length must be positive");
    }
    if (frequency <= 0.0f) {
        throw std::invalid_argument("frequency must be positive");
    }

    float* samples_ptr = Resonix::generateSamples(shape, sample_length, frequency);

    if (!samples_ptr) {
        throw std::runtime_error("Failed to generate samples");
    }

    size_t total_samples = static_cast<size_t>(sample_length) * Resonix::SAMPLE_RATE;

    py::capsule free_when_done(samples_ptr, [](void *f) {
        float *data = reinterpret_cast<float *>(f);
        delete[] data;
    });

    return py::array_t<float>(
        {static_cast<py::ssize_t>(total_samples)},
        {sizeof(float)},
        samples_ptr,
        free_when_done
    );
}

PYBIND11_MODULE(resonix, m) {
    m.doc() = "Resonix - Audio waveform generation library";

    m.attr("SAMPLE_RATE") = Resonix::SAMPLE_RATE;

    py::enum_<Resonix::Shape>(m, "Shape")
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