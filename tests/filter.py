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

print("\n=== Filter Statistics ===")
print(f"Original Signal RMS: {np.sqrt(np.mean(mixed_signal**2)):.4f}")
print(f"Lowpass Filtered RMS: {np.sqrt(np.mean(lowpass_filtered**2)):.4f}")
print(f"Highpass Filtered RMS: {np.sqrt(np.mean(highpass_filtered**2)):.4f}")
print(f"\nLowpass preserves {100*np.sqrt(np.mean(lowpass_filtered**2))/np.sqrt(np.mean(mixed_signal**2)):.1f}% of energy")
print(f"Highpass preserves {100*np.sqrt(np.mean(highpass_filtered**2))/np.sqrt(np.mean(mixed_signal**2)):.1f}% of energy")
