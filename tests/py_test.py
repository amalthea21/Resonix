import resonix
import numpy as np

# Test 1: Generate 1 second of 440Hz sine wave
print("Test 1: Generating sine wave...")
samples = resonix.generate_samples(resonix.Shape.SINE, 1, 440.0)
print(f"Generated {len(samples)} samples")
print(f"Sample rate: {resonix.SAMPLE_RATE}")
print(f"First 10 samples: {samples[:10]}")
print()

# Test 2: Generate sawtooth wave
print("Test 2: Generating sawtooth wave...")
sawtooth = resonix.generate_samples(resonix.Shape.SAWTOOTH, 1, 440.0)
print(f"Min: {sawtooth.min()}, Max: {sawtooth.max()}")
print(f"First 10 samples: {sawtooth[:10]}")
print()

# Test 3: Test all waveforms
print("Test 3: Testing all waveform types...")
shapes = [
    ('SINE', resonix.Shape.SINE),
    ('SQUARE', resonix.Shape.SQUARE),
    ('TRIANGLE', resonix.Shape.TRIANGLE),
    ('SAWTOOTH', resonix.Shape.SAWTOOTH),
    ('COSINE', resonix.Shape.COSINE),
    ('HANN', resonix.Shape.HANN),
    ('PHASED_HANN', resonix.Shape.PHASED_HANN),
]

for name, shape in shapes:
    samples = resonix.generate_samples(shape, 1, 440.0)
    print(f"{name:12s}: {len(samples)} samples, range [{samples.min():.3f}, {samples.max():.3f}]")

print("\nAll tests completed successfully!")