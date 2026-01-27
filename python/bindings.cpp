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

py::array_t<float> bandpassFilterNumPy(py::array_t<float> samples, float center_hz, float bandwidth_hz, float resonance = 0.707f) {
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

    if (center_hz <= 0.0f) {
        throw std::invalid_argument("center_hz must be positive");
    }
    if (bandwidth_hz <= 0.0f) {
        throw std::invalid_argument("bandwidth_hz must be positive");
    }
    if (resonance < 0.5f || resonance > 10.0f) {
        throw std::invalid_argument("resonance must be between 0.5 and 10.0");
    }

    std::unique_ptr<float[]> filtered_ptr = Resonix::bandpass_filter(input_ptr, sample_length, center_hz, bandwidth_hz, resonance);

    if (!filtered_ptr) {
        throw std::runtime_error("Failed to apply bandpass filter");
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

py::array_t<float> formantFilterNumPy(py::array_t<float> samples, float peak, float mix = 0.5f, float spread = 0.0f) {
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

    // Validate parameters
    if (peak < 0.0f || peak > 1.0f) {
        throw std::invalid_argument("peak must be between 0.0 and 1.0");
    }
    if (mix < 0.0f || mix > 1.0f) {
        throw std::invalid_argument("mix must be between 0.0 and 1.0");
    }
    if (spread < 0.0f || spread > 1.0f) {
        throw std::invalid_argument("spread must be between 0.0 and 1.0");
    }

    std::unique_ptr<float[]> filtered_ptr = Resonix::formant_filter(input_ptr, sample_length, peak, mix, spread);

    if (!filtered_ptr) {
        throw std::runtime_error("Failed to apply formant filter");
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

    m.def("bandpass_filter", &bandpassFilterNumPy,
          py::arg("samples"),
          py::arg("center_hz"),
          py::arg("bandwidth_hz"),
          py::arg("resonance") = 0.707f,
          R"pbdoc(
            Apply a bandpass filter to audio samples.

            Filters the input audio samples to isolate a specific frequency band by removing
            frequencies both above and below the specified range. Uses a second-order resonant
            bandpass filter implementation.

            Parameters
            ----------
            samples : numpy.ndarray
                1D array of float32 audio samples to filter
            center_hz : float
                Center frequency of the passband in Hz (e.g., 1000.0 for 1kHz center)
            bandwidth_hz : float
                Width of the passband in Hz (e.g., 200.0 for ±100Hz around center)
            resonance : float, optional
                Resonance/Q multiplier of the filter (default: 0.707 for moderate response)
                Higher values create sharper, more selective filtering.
                Should be between 0.5 and 10.0 for stability.

            Returns
            -------
            numpy.ndarray
                Array of filtered float32 samples with same length as input

            Notes
            -----
            The filter passes frequencies approximately from (center_hz - bandwidth_hz/2)
            to (center_hz + bandwidth_hz/2). Actual Q factor is calculated as:
            Q = (center_hz / bandwidth_hz) * resonance

            Examples
            --------
            >>> import resonix
            >>> samples = resonix.generate_samples(resonix.Shape.SINE, 1, 440.0)
            >>> # Isolate 750Hz - 1250Hz range
            >>> filtered = resonix.bandpass_filter(samples, 1000.0, 500.0)

            >>> # Create fricative 's' sound (4kHz - 8kHz)
            >>> noise = np.random.randn(44100).astype(np.float32)
            >>> fricative = resonix.bandpass_filter(noise, 6000.0, 4000.0, 0.5)

            >>> # Telephone effect (300Hz - 3400Hz)
            >>> phone = resonix.bandpass_filter(voice, 1850.0, 3100.0)
          )pbdoc");

    m.def("formant_filter", &formantFilterNumPy,
          py::arg("samples"),
          py::arg("peak"),
          py::arg("mix") = 0.5f,
          py::arg("spread") = 0.0f,
          R"pbdoc(
            Apply a formant filter to audio samples.

            Applies a series of resonant bandpass filters to simulate vowel formants,
            creating vocal-like or vowel-like timbres from the input audio.

            Parameters
            ----------
            samples : numpy.ndarray
                1D array of float32 audio samples to filter
            peak : float
                Determines which vowel formant to emphasize (0.0-1.0):
                0.0-0.2: "ah" as in "father"
                0.2-0.4: "eh" as in "bed"
                0.4-0.6: "ee" as in "see"
                0.6-0.8: "oh" as in "go"
                0.8-1.0: "oo" as in "boot"
            mix : float, optional
                Dry/wet mix (0.0 = dry, 1.0 = wet, default: 0.5)
            spread : float, optional
                Spread of formant frequencies (0.0 = normal, 1.0 = wide, default: 0.0)

            Returns
            -------
            numpy.ndarray
                Array of filtered float32 samples with same length as input

            Examples
            --------
            >>> import resonix
            >>> samples = resonix.generate_samples(resonix.Shape.SAWTOOTH, 1, 110.0)
            >>> # Create "ee" vowel sound
            >>> vowel_ee = resonix.formant_filter(samples, 0.5, 1.0, 0.0)

            >>> # Subtle vocal character with 50% mix
            >>> subtle = resonix.formant_filter(samples, 0.3, 0.5, 0.0)
          )pbdoc");
}