import resonix
import soundfile as sf

duration = 3
frequency = 110.0
original = resonix.generate_samples(resonix.Shape.SAWTOOTH, duration, frequency)

vowel_ah = resonix.formant_filter(original, peak=0.1, mix=1.0, spread=0.0)
vowel_eh = resonix.formant_filter(original, peak=0.3, mix=1.0, spread=0.0)
vowel_ee = resonix.formant_filter(original, peak=0.5, mix=1.0, spread=0.0)
vowel_oh = resonix.formant_filter(original, peak=0.7, mix=1.0, spread=0.0)
vowel_oo = resonix.formant_filter(original, peak=0.9, mix=1.0, spread=0.0)

sf.write('output/original.wav', original, resonix.SAMPLE_RATE)
sf.write('output/vowel_ah.wav', vowel_ah, resonix.SAMPLE_RATE)
sf.write('output/vowel_eh.wav', vowel_eh, resonix.SAMPLE_RATE)
sf.write('output/vowel_ee.wav', vowel_ee, resonix.SAMPLE_RATE)
sf.write('output/vowel_oh.wav', vowel_oh, resonix.SAMPLE_RATE)
sf.write('output/vowel_oo.wav', vowel_oo, resonix.SAMPLE_RATE)