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