# 🔊 **Resonix**
<sup> Python Sample-Generation Library <sup>

<br><br>

## 🛠️ **Building**

### **Prerequisites**
- CMake 3.15 or higher
- C++17 compatible compiler (GCC, Clang, or MSVC)
- Python 3.7+ (for Python bindings)
- pip (Python package manager)

### **Building Python Package (Recommended)**

1. **Clone the repository:**
```bash
   git clone https://github.com/amalthea21/Resonix
   cd Resonix
```

2. **Create and activate a virtual environment:**
```bash
   python3 -m venv venv
   source venv/bin/activate  # On Windows: venv\Scripts\activate
```

3. **Install dependencies and build:**
```bash
   pip install pybind11 numpy
   pip install -e .
```

4. **Verify installation:**
```bash
   python -c "import resonix; print(f'Resonix installed! Sample rate: {resonix.SAMPLE_RATE} Hz')"
```

### **Building C++ Library with CMake**

1. **Create build directory:**
```bash
   mkdir build && cd build
```

2. **Configure and build:**
```bash
   cmake ..
   make
```

3. **Optional - Build Python bindings with CMake:**
```bash
   cmake -DBUILD_PYTHON_BINDINGS=ON ..
   make
```

### **Building in CLion**

1. Open the project in CLion
2. CLion will automatically detect `CMakeLists.txt`
3. Click **Build → Build Project** or press `Ctrl+F9`
4. For Python support, configure the Python interpreter:
   - **File → Settings → Project → Python Interpreter**
   - Add your virtual environment: `venv/bin/python`

### **Running Tests**
```bash
# Activate virtual environment
source venv/bin/activate

# Run test scripts
python tests/plot.py
python tests/audio_generator.py
```

### **Custom Sample Rate**

To build with a different sample rate, define `RESONIX_SAMPLE_RATE`:
```bash
cmake -DRESONIX_SAMPLE_RATE=48000 ..
make
```

Or in your C++ code before including:
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
