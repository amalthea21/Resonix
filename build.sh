#!/bin/bash

# Resonix Clean Build Script
# This script removes all build artifacts and creates a fresh Python build

set -e  # Exit on error

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
find . -type f -name "*.pyd" -delete 2>/dev/null || true  # Windows

# Remove any generated output files from tests
echo "  → Removing test output files..."
rm -f output.wav 2>/dev/null || true

echo ""
echo "✨ Cleanup complete!"
echo ""

# Check if virtual environment exists
if [ ! -d "venv" ]; then
    echo "⚠️  Virtual environment not found. Creating one..."
    python3 -m venv venv
    echo "✅ Virtual environment created"
fi

# Activate virtual environment
echo "🔄 Activating virtual environment..."
source venv/bin/activate

# Upgrade pip
echo "📦 Upgrading pip..."
pip install --upgrade pip

# Install dependencies
echo "📦 Installing dependencies..."
pip install --force-reinstall --no-cache-dir numpy
pip install pybind11 matplotlib soundfile

# Build and install the package in editable mode
echo "🔨 Building Resonix..."
pip install -e .

echo ""
echo "✅ Build complete!"
echo ""
echo "🧪 Testing installation..."
python -c "import resonix; print(f'✓ Resonix installed successfully! Sample rate: {resonix.SAMPLE_RATE} Hz')"

echo ""
echo "💡 To activate the virtual environment in the future, run:"
echo "   source venv/bin/activate"