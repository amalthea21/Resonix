import resonix
import soundfile as sf

samples = resonix.generate_samples(resonix.Shape.SINE, 5, 164.0)
print(f"Generated {len(samples)} samples")
print(f"Sample rate: {resonix.SAMPLE_RATE}")

sf.write('output.wav', samples, resonix.SAMPLE_RATE)
print(f"Audio saved as 'output.wav'")