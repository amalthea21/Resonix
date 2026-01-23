import resonix
import numpy as np
import matplotlib.pyplot as plt

samples = resonix.generate_samples(resonix.Shape.PHASED_HANN, 1, 440.0)
print(f"Generated {len(samples)} samples")
print(f"Sample rate: {resonix.SAMPLE_RATE}")

duration = len(samples) / resonix.SAMPLE_RATE
time = np.linspace(0, duration, len(samples))

plt.figure(figsize=(12, 5))

plt.plot(time, samples, 'b-', linewidth=1, alpha=0.7)
plt.xlabel('Time (seconds)')
plt.ylabel('Amplitude')
plt.title('Generated Audio Samples')
plt.grid(True, alpha=0.3)

plt.figtext(0.02, 0.02,
            f'Samples: {len(samples)}, '
            f'Sample Rate: {resonix.SAMPLE_RATE} Hz, '
            f'Duration: {duration:.3f}s',
            fontsize=9, bbox=dict(facecolor='white', alpha=0.8))

plt.tight_layout()
plt.show()