import resonix
import soundfile as sf
import numpy as np
import os

SAMPLE_RATE = 44100
DURATION_PER_CHAR = 0.3
BASE_FREQUENCY = 220


def generate_vowel(char, duration_seconds):
    formant_peaks = {
        'a': 0.1,  # ah
        'e': 0.3,  # mid-low
        'i': 0.5,  # ee
        'o': 0.7,  # mid-high
        'u': 0.9  # oo
    }

    peak = formant_peaks.get(char, 0.4)

    samples = resonix.generate_samples(
        resonix.Shape.SAWTOOTH,
        1,
        BASE_FREQUENCY,
    )

    target_samples = int(duration_seconds * SAMPLE_RATE)
    samples = samples[:target_samples]

    return resonix.formant_filter(samples, peak=peak, mix=1.0, spread=0.0)


def generate_nasal(char, duration_seconds):
    samples = resonix.generate_samples(
        resonix.Shape.SAWTOOTH,
        1,
        BASE_FREQUENCY * 0.8,
    )

    target_samples = int(duration_seconds * SAMPLE_RATE)
    samples = samples[:target_samples]

    filtered = resonix.lowpass_filter(samples, cutoff_hz=1500, resonance=0.5)
    return filtered * 0.6


def generate_fricative(char, duration_seconds, voiced=False):
    samples_count = int(duration_seconds * SAMPLE_RATE)

    if voiced:
        buzz = resonix.generate_samples(
            resonix.Shape.SAWTOOTH,
            1,
            BASE_FREQUENCY
        )
        target_samples = int(duration_seconds * SAMPLE_RATE)
        buzz = buzz[:target_samples]

        noise = np.random.randn(len(buzz)).astype(np.float32) * 0.3
        mixed = buzz * 0.4 + noise * 0.6
    else:
        mixed = np.random.randn(samples_count).astype(np.float32)

    fricative_cutoffs = {
        's': 5000,
        'z': 4500,
        'sh': 3000,
        'f': 6000,
        'v': 3500,
        'th': 5500,
        'h': 2000
    }

    cutoff = fricative_cutoffs.get(char, 4000)
    filtered = resonix.highpass_filter(mixed, cutoff_hz=cutoff, resonance=0.5)

    return filtered * 0.5


def generate_plosive(char, duration_seconds, voiced=False):
    total_samples = int(duration_seconds * SAMPLE_RATE)

    closure_duration = int(0.06 * SAMPLE_RATE)
    silence = np.zeros(closure_duration, dtype=np.float32)

    burst_duration = int(0.02 * SAMPLE_RATE)
    burst = np.random.randn(burst_duration).astype(np.float32) * 0.8

    plosive_cutoffs = {
        'p': 2000,
        'b': 1500,
        't': 4000,
        'd': 3000,
        'k': 3500,
        'g': 2500
    }

    cutoff = plosive_cutoffs.get(char, 3000)
    burst = resonix.highpass_filter(burst, cutoff_hz=cutoff, resonance=0.5)

    if voiced:
        voice_duration = total_samples - closure_duration - burst_duration
        if voice_duration > 0:
            voicing = resonix.generate_samples(
                resonix.Shape.SAWTOOTH,
                voice_duration / SAMPLE_RATE,
                BASE_FREQUENCY
            )
            return np.concatenate([silence, burst, voicing])

    remaining = total_samples - closure_duration - burst_duration
    padding = np.zeros(max(0, remaining), dtype=np.float32)

    return np.concatenate([silence, burst, padding])


def crossfade(sound1, sound2, fade_samples=100):
    if len(sound1) < fade_samples or len(sound2) < fade_samples:
        return np.concatenate([sound1, sound2])

    fade_out = np.linspace(1, 0, fade_samples)
    fade_in = np.linspace(0, 1, fade_samples)

    overlapped = sound1[-fade_samples:] * fade_out + sound2[:fade_samples] * fade_in

    return np.concatenate([sound1[:-fade_samples], overlapped, sound2[fade_samples:]])

def main():
    text = input("Text: ").lower()
    samples_list = []

    vowels = 'aeiou'
    nasals = 'mn'
    voiceless_fricatives = 'fsth'
    voiced_fricatives = 'vzh'
    voiceless_plosives = 'ptk'
    voiced_plosives = 'bdg'

    i = 0
    previous_sound = None

    while i < len(text):
        char = text[i]
        current_sound = None

        if char == 'c':
            if i + 1 < len(text):
                next_char = text[i + 1]
                if next_char in 'eiy':
                    char = 's'  # soft C
                else:
                    char = 'k'  # hard C
            else:
                char = 'k'

        if char == ' ':
            current_sound = np.zeros(int(0.1 * SAMPLE_RATE), dtype=np.float32)

        elif char in vowels:
            current_sound = generate_vowel(char, DURATION_PER_CHAR)

        elif char in nasals:
            current_sound = generate_nasal(char, DURATION_PER_CHAR)

        elif char in voiceless_fricatives:
            if char == 't' and i + 1 < len(text) and text[i + 1] == 'h':
                current_sound = generate_fricative('th', DURATION_PER_CHAR, voiced=False)
                i += 1
            else:
                current_sound = generate_fricative(char, DURATION_PER_CHAR, voiced=False)

        elif char in voiced_fricatives:
            current_sound = generate_fricative(char, DURATION_PER_CHAR, voiced=True)

        elif char in voiceless_plosives:
            current_sound = generate_plosive(char, DURATION_PER_CHAR, voiced=False)

        elif char in voiced_plosives:
            current_sound = generate_plosive(char, DURATION_PER_CHAR, voiced=True)

        else:
            current_sound = np.zeros(int(0.05 * SAMPLE_RATE), dtype=np.float32)

        if previous_sound is not None and current_sound is not None:
            if len(current_sound) > 100 and len(previous_sound) > 100:
                fade_samples = int(0.015 * SAMPLE_RATE)
                transitioned = crossfade(previous_sound, current_sound, fade_samples)
                samples_list[-1] = transitioned
            else:
                samples_list.append(current_sound)
        else:
            samples_list.append(current_sound)

        previous_sound = current_sound
        i += 1

    if samples_list:
        final_samples = np.concatenate(samples_list)

        os.makedirs("output", exist_ok=True)
        sf.write("output/tts_output.wav", final_samples, SAMPLE_RATE)

        print("Test finished")
        print(f"Duration: {len(final_samples) / SAMPLE_RATE:.2f} seconds")
    else:
        print("No audio generated")


if __name__ == "__main__":
    main()