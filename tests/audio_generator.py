import resonix
import soundfile as sf

samples = resonix.generate_samples(resonix.Shape.PHASED_HANN, 5, 440.0)

print(f"Generated {len(samples)} samples")
print(f"Sample rate: {resonix.SAMPLE_RATE}")

sf.write('output/audio.wav', samples, resonix.SAMPLE_RATE)
print(f"Audio saved as 'output.wav'")