import resonix
import soundfile as sf
import numpy as np

text = input("Text: ").lower()

duration_seconds_per_char = 0.2
frequency = 220.0

samples_per_char = int(duration_seconds_per_char * resonix.SAMPLE_RATE)

samples_list = []

for char in text:
    if char == 'a':
        peak = 0.1
    elif char == 'e':
        peak = 0.3
    elif char == 'i':
        peak = 0.5
    elif char == 'o':
        peak = 0.7
    elif char == 'u':
        peak = 0.9
    elif char == ' ':
        samples_list.append(np.zeros(samples_per_char, dtype=np.float32))
        continue
    else:
        peak = 0.4

    original = resonix.generate_samples(resonix.Shape.SAWTOOTH, samples_per_char, frequency)
    filtered = resonix.formant_filter(original, peak=peak, mix=1.0, spread=0.0)
    samples_list.append(filtered)

if samples_list:
    final_samples = np.concatenate(samples_list)

    sf.write('output/tts_output.wav', final_samples, resonix.SAMPLE_RATE)
    print(f"Saved to output/tts_output.wav")
else:
    print("No text to process")