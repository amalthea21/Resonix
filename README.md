# 🔊 **Resonix**

Resonix is a lightweight, high-performance audio sample generation library designed for flexibility and ease of use across C++ and Python environments. It provides tools for generating, processing, and analyzing audio samples, with support for custom build configurations, multiple sample rates, and example workflows to quickly get you started. Whether you’re building audio tools, experimenting with signal processing, or integrating sound features into your applications, Resonix gives you a solid foundation to work from.

<br><br>

## 🚀 **Features**

- Audio sample generation with configurable parameters
- C++ core with python bindings (you can also use it as C++ Library!)
- Custom sample rate
- Minimal dependencies

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
cd Resonix/scripts

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

To build just the C++ library without Python bindings, run these commands:

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

## 🐍 **Examples**

### [Note Plot](tests/plot.py)

<img width="1784" height="731" alt="grafik" src="https://github.com/user-attachments/assets/66d2207e-9262-41e6-91ce-9d7343fcacca" />

<br><br>

### [Audio Generator](tests/audio_generator.py)

**Output**: A 5 second long WAV file, playing the A4 Note

### [Filter Plot](tests/filter.py)

<img width="2234" height="1481" alt="grafik" src="https://github.com/user-attachments/assets/a49785a6-42ac-40d4-9e14-e870138c0596" />

<br><br>

### [Memory Test](tests/memory_analysis.py)

<img width="2338" height="1609" alt="grafik" src="https://github.com/user-attachments/assets/8c1da8b1-3614-44bf-ae1c-37930dca88f0" />

<br><br>
