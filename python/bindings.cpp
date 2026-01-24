#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <stdexcept>
#include <memory>
#include "Resonix.hpp"

namespace py = pybind11;

py::array_t<float> generateSamplesNumPy(Resonix::Shape shape, int sample_length, float frequency) {
    if (sample_length <= 0) {
        throw std::invalid_argument("sample_length must be positive");
    }
    if (frequency <= 0.0f) {
        throw std::invalid_argument("frequency must be positive");
    }

    std::unique_ptr<float[]> samples_ptr = Resonix::generateSamples(shape, sample_length, frequency);

    if (!samples_ptr) {
        throw std::runtime_error("Failed to generate samples");
    }

    size_t total_samples = static_cast<size_t>(sample_length) * Resonix::SAMPLE_RATE;

    float* raw_ptr = samples_ptr.release();

    auto cleanup = [](void *f) {
        if (f) {
            float *data = static_cast<float*>(f);
            delete[] data;
        }
    };

    py::capsule free_when_done(raw_ptr, cleanup);

    return py::array_t<float>(
        {static_cast<py::ssize_t>(total_samples)},
        {sizeof(float)},
        raw_ptr,
        free_when_done
    );
}

py::array_t<float> lowpassFilterNumPy(py::array_t<float> samples, float cutoff_hz, float resonance = 0.707f) {
    py::buffer_info buf = samples.request();

    if (buf.ndim != 1) {
        throw std::invalid_argument("samples must be a 1D array");
    }
    if (buf.size == 0) {
        throw std::invalid_argument("samples array cannot be empty");
    }
    if (buf.format != py::format_descriptor<float>::format()) {
        throw std::invalid_argument("samples must be float32 array");
    }

    float* input_ptr = static_cast<float*>(buf.ptr);
    int sample_length = static_cast<int>(buf.size);

    if (cutoff_hz <= 0.0f) {
        throw std::invalid_argument("cutoff_hz must be positive");
    }
    if (resonance < 0.5f || resonance > 10.0f) {
        throw std::invalid_argument("resonance must be between 0.5 and 10.0");
    }

    std::unique_ptr<float[]> filtered_ptr = Resonix::lowpass_filter(input_ptr, sample_length, cutoff_hz, resonance);

    if (!filtered_ptr) {
        throw std::runtime_error("Failed to apply lowpass filter");
    }

    float* raw_ptr = filtered_ptr.release();

    auto cleanup = [](void *f) {
        if (f) {
            float *data = static_cast<float*>(f);
            delete[] data;
        }
    };

    py::capsule free_when_done(raw_ptr, cleanup);

    return py::array_t<float>(
        {static_cast<py::ssize_t>(sample_length)},
        {sizeof(float)},
        raw_ptr,
        free_when_done
    );
}

py::array_t<float> highpassFilterNumPy(py::array_t<float> samples, float cutoff_hz, float resonance = 0.707f) {
    py::buffer_info buf = samples.request();

    if (buf.ndim != 1) {
        throw std::invalid_argument("samples must be a 1D array");
    }
    if (buf.size == 0) {
        throw std::invalid_argument("samples array cannot be empty");
    }
    if (buf.format != py::format_descriptor<float>::format()) {
        throw std::invalid_argument("samples must be float32 array");
    }

    float* input_ptr = static_cast<float*>(buf.ptr);
    int sample_length = static_cast<int>(buf.size);

    if (cutoff_hz <= 0.0f) {
        throw std::invalid_argument("cutoff_hz must be positive");
    }
    if (resonance < 0.5f || resonance > 10.0f) {
        throw std::invalid_argument("resonance must be between 0.5 and 10.0");
    }

    std::unique_ptr<float[]> filtered_ptr = Resonix::highpass_filter(input_ptr, sample_length, cutoff_hz, resonance);

    if (!filtered_ptr) {
        throw std::runtime_error("Failed to apply highpass filter");
    }

    float* raw_ptr = filtered_ptr.release();

    // Create capsule with explicit cleanup and null check
    auto cleanup = [](void *f) {
        if (f) {
            float *data = static_cast<float*>(f);
            delete[] data;
        }
    };

    py::capsule free_when_done(raw_ptr, cleanup);

    return py::array_t<float>(
        {static_cast<py::ssize_t>(sample_length)},
        {sizeof(float)},
        raw_ptr,
        free_when_done
    );
}

PYBIND11_MODULE(resonix, m) {
    m.doc() = "Resonix - Audio waveform generation and processing library";

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

    m.def("lowpass_filter", &lowpassFilterNumPy,
          py::arg("samples"),
          py::arg("cutoff_hz"),
          py::arg("resonance") = 0.707f,
          R"pbdoc(
            Apply a lowpass filter to audio samples.

            Filters the input audio samples with a second-order resonant lowpass filter,
            removing high frequencies above the specified cutoff point while preserving
            lower frequencies.

            Parameters
            ----------
            samples : numpy.ndarray
                1D array of float32 audio samples to filter
            cutoff_hz : float
                Cutoff frequency in Hz (e.g., 1000.0 for 1kHz lowpass)
            resonance : float, optional
                Resonance/Q factor of the filter (default: 0.707 for Butterworth response)
                Higher values create a resonant peak near the cutoff frequency.
                Should be between 0.5 and 10.0 for stability.

            Returns
            -------
            numpy.ndarray
                Array of filtered float32 samples with same length as input

            Examples
            --------
            >>> import resonix
            >>> samples = resonix.generate_samples(resonix.Shape.SINE, 1, 440.0)
            >>> filtered = resonix.lowpass_filter(samples, 1000.0)
          )pbdoc");

    m.def("highpass_filter", &highpassFilterNumPy,
          py::arg("samples"),
          py::arg("cutoff_hz"),
          py::arg("resonance") = 0.707f,
          R"pbdoc(
            Apply a highpass filter to audio samples.

            Filters the input audio samples with a second-order resonant highpass filter,
            removing low frequencies below the specified cutoff point while preserving
            higher frequencies.

            Parameters
            ----------
            samples : numpy.ndarray
                1D array of float32 audio samples to filter
            cutoff_hz : float
                Cutoff frequency in Hz (e.g., 200.0 for 200Hz highpass)
            resonance : float, optional
                Resonance/Q factor of the filter (default: 0.707 for Butterworth response)
                Higher values create a resonant peak near the cutoff frequency.
                Should be between 0.5 and 10.0 for stability.

            Returns
            -------
            numpy.ndarray
                Array of filtered float32 samples with same length as input

            Examples
            --------
            >>> import resonix
            >>> samples = resonix.generate_samples(resonix.Shape.SINE, 1, 440.0)
            >>> filtered = resonix.highpass_filter(samples, 200.0)
          )pbdoc");
}