from setuptools import setup, Extension
import subprocess
import sys

def get_pybind_include():
    try:
        import pybind11
        return pybind11.get_include()
    except ImportError:
        # Fallback: try to get it from pip show
        result = subprocess.run(
            [sys.executable, '-m', 'pip', 'show', 'pybind11'],
            capture_output=True, text=True
        )
        if result.returncode != 0:
            raise RuntimeError("pybind11 is not installed. Install it with: pip install pybind11")
        for line in result.stdout.split('\n'):
            if line.startswith('Location:'):
                location = line.split(':', 1)[1].strip()
                return location + '/pybind11/include'
        raise RuntimeError("Could not find pybind11 include directory")

ext_modules = [
    Extension(
        'resonix',
        sources=[
            'python/bindings.cpp',
            'src/Resonix.cpp',
            'src/generator/Trigonometric.cpp',
            'src/generator/Primitives.cpp',
            'src/generator/Hann.cpp',
            'src/math/Trigonometry.cpp',
            'src/math/Utils.cpp',
            'src/math/NaN.cpp',
        ],
        include_dirs=[
            get_pybind_include(),
            'include',
        ],
        language='c++',
        extra_compile_args=['-std=c++26'],
    ),
]

setup(
    name='resonix',
    version='0.1.0',
    author='Your Name',
    description='Audio waveform generation library',
    long_description='',
    ext_modules=ext_modules,
    setup_requires=['pybind11>=2.6.0'],
    install_requires=['pybind11>=2.6.0', 'numpy'],
    python_requires='>=3.7',
    zip_safe=False,
)