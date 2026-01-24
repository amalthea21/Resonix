# 🔊 **Resonix**
<sup> Python Sample-Generation Library <sup>

<br><br>

## 🛠️ **Building**

### **Prerequisites**

- CMake 3.15 or higher
- C++17 compatible compiler (GCC, Clang or MSVC)
- Python 3.7+
- pip (Python package manager)

### **Quick Start**

The easiest way to build Resonix is using the provided build script:

```sh
# Clone the repository
git clone https://github.com/amalthea21/Resonix
cd Resonix

# Make the build script executable (Unix/Linux/macOS only)
chmod +x build.sh

# Run the automated build script
./build.sh
```

The build script will:

- Clean all previous build artifacts
- Create a fresh virtual environment in venv/
- Install all dependencies (numpy, pybind11, matplotlib, soundfile)
- Build and install Resonix in editable mode
- Verify the installation

After the script completes, activate the virtual environment:

```sh
source venv/bin/activate  # On Windows: venv\Scripts\activate
```

### **Building C++ Library Only**

To build just the C++ library without Python bindings:

```sh
mkdir build && cd build
cmake -DBUILD_PYTHON_BINDINGS=OFF ..
make
cmake -DBUILD_PYTHON_BINDINGS=ON ..
make
```

### **Custom Sample Rate**

To build with a different sample rate (default is 44100 Hz):

**Option 1: CMake**

```sh
mkdir build && cd build
cmake -DRESONIX_SAMPLE_RATE=48000 ..
make
```

**Option 2: C++ Preprocessor**

```cpp
#define RESONIX_SAMPLE_RATE 48000
#include "Resonix.hpp"
```

## 🐍 **Example Usage**

### Plot

```py
import resonix
import matplotlib.pyplot as plt
import numpy as np

samples = resonix.generate_samples(resonix.Shape.HANN, 1, 440.0)

time = np.linspace(0, 1, len(samples))

plt.figure(figsize=(12, 5))

plt.subplot(1, 2, 1)
plt.plot(time, samples)
plt.title('Generated Samples - Time Domain')
plt.xlabel('Time (seconds)')
plt.ylabel('Amplitude')
plt.grid(True)

plt.subplot(1, 2, 2)
plt.plot(time[:1000], samples[:1000])  # First 1000 samples
plt.title('First 1000 Samples - Zoomed')
plt.xlabel('Time (seconds)')
plt.ylabel('Amplitude')
plt.grid(True)

plt.tight_layout()
plt.show()
```

**Output**:

<img width="1402" height="1620" alt="grafik" src="https://github.com/user-attachments/assets/037d9972-e2db-4d55-9981-5e81a120710c" />

### Sound Generation

```py
import resonix
import soundfile as sf

samples = resonix.generate_samples(resonix.Shape.SINE, 5, 440)
print(f"Generated {len(samples)} samples")
print(f"Sample rate: {resonix.SAMPLE_RATE}")

sf.write('output.wav', samples, resonix.SAMPLE_RATE)
print(f"Audio saved as 'output.wav'")
```

**Output**: A 5 second long WAV file, playing the A4 Note

### Filter Plotting

```py
import resonix
import matplotlib.pyplot as plt
import numpy as np

duration = 2
sample_rate = resonix.SAMPLE_RATE

low_freq = resonix.generate_samples(resonix.Shape.SINE, duration, 100.0)
mid_freq = resonix.generate_samples(resonix.Shape.SINE, duration, 440.0)
high_freq = resonix.generate_samples(resonix.Shape.SINE, duration, 3000.0)

mixed_signal = low_freq * 0.3 + mid_freq * 0.5 + high_freq * 0.3

lowpass_filtered = resonix.lowpass_filter(mixed_signal, 500.0, 0.707)
highpass_filtered = resonix.highpass_filter(mixed_signal, 500.0, 0.707)

time = np.linspace(0, duration, len(mixed_signal))

fig, axs = plt.subplots(2, 2, figsize=(15, 10))

axs[0, 0].plot(time[:1000], mixed_signal[:1000], 'b-', linewidth=0.8)
axs[0, 0].set_title('Original Signal (100Hz + 440Hz + 3000Hz)')
axs[0, 0].set_xlabel('Time (seconds)')
axs[0, 0].set_ylabel('Amplitude')
axs[0, 0].grid(True, alpha=0.3)

# Plot 2: Lowpass filtered (time domain)
axs[0, 1].plot(time[:1000], lowpass_filtered[:1000], 'g-', linewidth=0.8)
axs[0, 1].set_title('Lowpass Filter (Cutoff: 500Hz) - Keeps Bass & Mid')
axs[0, 1].set_xlabel('Time (seconds)')
axs[0, 1].set_ylabel('Amplitude')
axs[0, 1].grid(True, alpha=0.3)

# Plot 3: Highpass filtered (time domain)
axs[1, 0].plot(time[:1000], highpass_filtered[:1000], 'r-', linewidth=0.8)
axs[1, 0].set_title('Highpass Filter (Cutoff: 500Hz) - Keeps Treble')
axs[1, 0].set_xlabel('Time (seconds)')
axs[1, 0].set_ylabel('Amplitude')
axs[1, 0].grid(True, alpha=0.3)

def compute_fft(signal):
    fft_result = np.fft.fft(signal)
    freqs = np.fft.fftfreq(len(signal), 1/sample_rate)
    magnitude = np.abs(fft_result)
    # Only take positive frequencies
    positive_freqs = freqs[:len(freqs)//2]
    positive_magnitude = magnitude[:len(magnitude)//2]
    return positive_freqs, positive_magnitude

freqs_orig, mag_orig = compute_fft(mixed_signal)
freqs_low, mag_low = compute_fft(lowpass_filtered)
freqs_high, mag_high = compute_fft(highpass_filtered)

axs[1, 1].semilogy(freqs_orig, mag_orig, 'b-', alpha=0.6, linewidth=1, label='Original')
axs[1, 1].semilogy(freqs_low, mag_low, 'g-', alpha=0.8, linewidth=1.5, label='Lowpass')
axs[1, 1].semilogy(freqs_high, mag_high, 'r-', alpha=0.8, linewidth=1.5, label='Highpass')
axs[1, 1].axvline(x=500, color='orange', linestyle='--', linewidth=2, label='Cutoff (500Hz)')
axs[1, 1].set_xlim(0, 5000)
axs[1, 1].set_title('Frequency Spectrum Comparison')
axs[1, 1].set_xlabel('Frequency (Hz)')
axs[1, 1].set_ylabel('Magnitude (log scale)')
axs[1, 1].legend()
axs[1, 1].grid(True, alpha=0.3)

plt.tight_layout()
plt.savefig('filter_comparison.png', dpi=150, bbox_inches='tight')
print("✓ Filter comparison plot saved as 'filter_comparison.png'")
plt.show()

print("\n=== Filter Statistics ===")
print(f"Original Signal RMS: {np.sqrt(np.mean(mixed_signal**2)):.4f}")
print(f"Lowpass Filtered RMS: {np.sqrt(np.mean(lowpass_filtered**2)):.4f}")
print(f"Highpass Filtered RMS: {np.sqrt(np.mean(highpass_filtered**2)):.4f}")
print(f"\nLowpass preserves {100*np.sqrt(np.mean(lowpass_filtered**2))/np.sqrt(np.mean(mixed_signal**2)):.1f}% of energy")
print(f"Highpass preserves {100*np.sqrt(np.mean(highpass_filtered**2))/np.sqrt(np.mean(mixed_signal**2)):.1f}% of energy")
```

**Output**:

<img width="1410" height="1617" alt="grafik" src="https://github.com/user-attachments/assets/757133f5-c19b-42d0-950a-ef6eba4609ac" />

### Memory Test

[Code](tests/memory_analysis.py)

**Output**:

<img width="2244" height="1526" alt="memory_analysis" src="https://github.com/user-attachments/assets/590e3398-f869-4904-9926-1926851fbbc5" />

