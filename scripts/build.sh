#!/bin/bash

# Resonix Clean Build Script
# This script removes all build artifacts and creates a fresh Python build

set -e

# Get the project root directory (parent of scripts/)
PROJECT_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$PROJECT_ROOT"

echo "🧹 Cleaning Resonix build artifacts..."

# Remove Python build directories
echo "  → Removing Python build artifacts..."
rm -rf build/
rm -rf dist/
rm -rf *.egg-info/
rm -rf resonix.egg-info/

# Remove CMake build directories
echo "  → Removing CMake build directories..."
rm -rf cmake-build-*/

# Remove Python cache
echo "  → Removing Python cache..."
find . -type d -name "__pycache__" -exec rm -rf {} + 2>/dev/null || true
find . -type f -name "*.pyc" -delete 2>/dev/null || true
find . -type f -name "*.pyo" -delete 2>/dev/null || true

# Remove compiled Python extensions
echo "  → Removing compiled extensions..."
find . -type f -name "*.so" -delete 2>/dev/null || true
find . -type f -name "*.pyd" -delete 2>/dev/null || true

# Remove any generated output files from tests
echo "  → Removing test output files..."
rm -f output.wav 2>/dev/null || true
rm -rf tests/output 2>/dev/null || true

echo ""
echo "✨ Cleanup complete!"
echo ""

# Remove and recreate virtual environment
if [ -d "venv" ]; then
    echo "🗑️  Removing existing virtual environment..."
    rm -rf venv
fi

echo "🆕 Creating fresh virtual environment..."
python3 -m venv venv
echo "✅ Virtual environment created"

# Activate virtual environment
echo "🔄 Activating virtual environment..."
source venv/bin/activate

# Upgrade pip and setuptools
echo "📦 Upgrading pip, setuptools, and wheel..."
pip install --upgrade pip setuptools wheel

# Install dependencies
echo "📦 Installing dependencies..."
pip install numpy
pip install pybind11
pip install matplotlib
pip install soundfile

# Build and install the package in editable mode
echo "🔨 Building Resonix..."
pip install -e .

echo ""
echo "✅ Build complete!"
echo ""
echo "🧪 Testing installation..."
python -c "import resonix; print(f'✓ Resonix installed successfully! Sample rate: {resonix.SAMPLE_RATE} Hz')"
python -c "import numpy; print(f'✓ NumPy installed successfully!')"
python -c "import matplotlib.pyplot as plt; print(f'✓ Matplotlib installed successfully!')"
python -c "import soundfile; print(f'✓ SoundFile installed successfully!')"

echo ""
echo "💡 To activate the virtual environment, run:"
echo "   source venv/bin/activate"
echo ""
echo "📚 See README.md for detailed documentation and examples"