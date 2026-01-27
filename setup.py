from setuptools import setup, Extension
import os

def get_pybind_include():
    try:
        import pybind11
        return pybind11.get_include()
    except ImportError:
        raise RuntimeError("pybind11 is not installed. Install it with: pip install pybind11")

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
            'src/math/WindowFunctions.cpp',
            'src/Filter/FormantFilter.cpp',
            'src/Filter/PassFilter.cpp',
            'src/Filter/BandpassFilter.cpp',
        ],
        include_dirs=[
            get_pybind_include(),
            'include',
        ],
        language='c++',
        extra_compile_args=['-std=c++17'],
    ),
]

setup(
    name='resonix',
    version='0.1.0',
    author='Your Name',
    description='Audio waveform generation library',
    ext_modules=ext_modules,
    install_requires=['pybind11>=2.6.0', 'numpy'],
    python_requires='>=3.7',
    zip_safe=False,
    options={
        'build': {
            'build_lib': '.'  # Build in project root, not src/
        }
    }
)