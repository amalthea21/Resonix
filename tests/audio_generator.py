import resonix
import soundfile as sf

samples = resonix.generate_samples(resonix.Shape.SAWTOOTH, 5, 440.0)
filtered_samples = resonix.formant_filter(samples, peak=0.4, mix=0.8, spread=0.2)

sf.write('output/audio.wav', samples, resonix.SAMPLE_RATE)

print('Test finished')