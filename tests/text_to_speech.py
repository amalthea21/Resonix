import resonix
import soundfile as sf
import numpy as np
import os

text = input("Text: ").lower()

duration_per_char = 0.2
frequency = 220.0
SAMPLE_RATE = 44100

TARGET_SAMPLES = int(duration_per_char * SAMPLE_RATE)

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
        samples_list.append(
            np.zeros(int(0.1 * SAMPLE_RATE), dtype=np.float32)
        )
        continue
    else:
        peak = 0.4

    original = resonix.generate_samples(
        resonix.Shape.SAWTOOTH,
        1,
        frequency
    )

    # Trim to desired duration
    original = original[:TARGET_SAMPLES]

    filtered = resonix.formant_filter(
        original,
        peak=peak,
        mix=1.0,
        spread=0.0
    )

    samples_list.append(filtered)

if samples_list:
    final_samples = np.concatenate(samples_list)

    os.makedirs("output", exist_ok=True)
    sf.write("output/tts_output.wav", final_samples, SAMPLE_RATE)

    print("Saved to output/tts_output.wav")
    print(f"Duration: {len(final_samples) / SAMPLE_RATE:.2f} seconds")
