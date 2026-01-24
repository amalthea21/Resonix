import resonix
import soundfile as sf

samples = resonix.generate_samples(resonix.Shape.PHASED_HANN, 5, 440.0)

sf.write('output/audio.wav', samples, resonix.SAMPLE_RATE)

print('Test finished')