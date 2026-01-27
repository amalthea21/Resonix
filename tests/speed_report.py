import resonix
import numpy as np
import os
import matplotlib.pyplot as plt
import time

os.makedirs('output', exist_ok=True)

shapes = [
    resonix.Shape.SINE,
    resonix.Shape.SQUARE,
    resonix.Shape.TRIANGLE,
    resonix.Shape.SAWTOOTH,
    resonix.Shape.COSINE,
    resonix.Shape.TANGENT,
    resonix.Shape.COTANGENT,
    resonix.Shape.HANN,
    resonix.Shape.PHASED_HANN
]

shape_names = ['SINE', 'SQUARE', 'TRIANGLE', 'SAWTOOTH', 'COSINE', 'TANGENT', 'COTANGENT', 'HANN', 'PHASED_HANN']

iterations = 50
duration = 1
frequency = 440.0

shape_times = {shape: [] for shape in shapes}

for iteration in range(iterations):
    for shape in shapes:
        start = time.perf_counter()
        samples = resonix.generate_samples(shape, duration, frequency)
        end = time.perf_counter()
        shape_times[shape].append((end - start) * 1000)
        del samples

avg_times = []
std_times = []
for shape in shapes:
    avg_times.append(np.mean(shape_times[shape]))
    std_times.append(np.std(shape_times[shape]))

filter_iterations = 50
filter_types = {
    'lowpass': lambda s: resonix.lowpass_filter(s, 1000.0, 0.707),
    'highpass': lambda s: resonix.highpass_filter(s, 1000.0, 0.707),
    'bandpass': lambda s: resonix.bandpass_filter(s, 1000.0, 500.0, 0.707),
    'formant': lambda s: resonix.formant_filter(s, 0.5, 0.8, 0.2)
}

filter_times = {ftype: [] for ftype in filter_types.keys()}

for iteration in range(filter_iterations):
    base_samples = resonix.generate_samples(resonix.Shape.SINE, duration, frequency)

    for ftype, func in filter_types.items():
        start = time.perf_counter()
        filtered = func(base_samples)
        end = time.perf_counter()
        filter_times[ftype].append((end - start) * 1000)
        del filtered

    del base_samples

avg_filter_times = []
std_filter_times = []
filter_labels = []
for ftype in filter_types.keys():
    avg_filter_times.append(np.mean(filter_times[ftype]))
    std_filter_times.append(np.std(filter_times[ftype]))
    filter_labels.append(ftype.capitalize())

duration_tests = [1, 2, 5, 10]
duration_speed = {shape: [] for shape in shapes}

for duration_test in duration_tests:
    for shape in shapes:
        times = []
        for _ in range(20):
            start = time.perf_counter()
            samples = resonix.generate_samples(shape, duration_test, frequency)
            end = time.perf_counter()
            times.append((end - start) * 1000)
            del samples
        duration_speed[shape].append(np.mean(times))

frequency_tests = [110.0, 220.0, 440.0, 880.0, 1760.0]
frequency_speed = []

for freq in frequency_tests:
    times = []
    for _ in range(30):
        start = time.perf_counter()
        samples = resonix.generate_samples(resonix.Shape.SINE, 1, freq)
        end = time.perf_counter()
        times.append((end - start) * 1000)
        del samples
    frequency_speed.append(np.mean(times))

fig, axs = plt.subplots(2, 2, figsize=(16, 12))

colors = plt.cm.tab10(np.linspace(0, 1, len(shapes)))
bars = axs[0, 0].bar(range(len(shapes)), avg_times, yerr=std_times, color=colors,
                     alpha=0.8, edgecolor='black', capsize=5)
axs[0, 0].set_title('Average Generation Time per Waveform', fontweight='bold', fontsize=12)
axs[0, 0].set_xlabel('Waveform Type')
axs[0, 0].set_ylabel('Time (ms)')
axs[0, 0].set_xticks(range(len(shapes)))
axs[0, 0].set_xticklabels(shape_names, rotation=45, ha='right', fontsize=9)
axs[0, 0].grid(True, alpha=0.3, axis='y')

for i, (bar, avg_time) in enumerate(zip(bars, avg_times)):
    height = bar.get_height()
    axs[0, 0].text(bar.get_x() + bar.get_width() / 2., height,
                   f'{avg_time:.2f}', ha='center', va='bottom', fontsize=8)

filter_colors = ['steelblue', 'coral', 'mediumseagreen', 'orchid']
bars = axs[0, 1].bar(range(len(filter_types)), avg_filter_times, yerr=std_filter_times,
                     color=filter_colors, alpha=0.8, edgecolor='black', capsize=5)
axs[0, 1].set_title('Average Filter Processing Time', fontweight='bold', fontsize=12)
axs[0, 1].set_xlabel('Filter Type')
axs[0, 1].set_ylabel('Time (ms)')
axs[0, 1].set_xticks(range(len(filter_types)))
axs[0, 1].set_xticklabels(filter_labels, rotation=45, ha='right')
axs[0, 1].grid(True, alpha=0.3, axis='y')

for i, (bar, avg_time) in enumerate(zip(bars, avg_filter_times)):
    height = bar.get_height()
    axs[0, 1].text(bar.get_x() + bar.get_width() / 2., height,
                   f'{avg_time:.2f}', ha='center', va='bottom', fontsize=9)

for i, shape in enumerate(shapes):
    axs[1, 0].plot(duration_tests, duration_speed[shape], marker='o',
                   label=shape_names[i], linewidth=2, alpha=0.8)

axs[1, 0].set_title('Generation Time vs Duration', fontweight='bold', fontsize=12)
axs[1, 0].set_xlabel('Duration (seconds)')
axs[1, 0].set_ylabel('Time (ms)')
axs[1, 0].legend(loc='upper left', fontsize=8, ncol=2)
axs[1, 0].grid(True, alpha=0.3)

axs[1, 1].plot(frequency_tests, frequency_speed, marker='o', color='darkred',
               linewidth=2.5, markersize=8, alpha=0.8)
axs[1, 1].set_title('Generation Time vs Frequency (SINE)', fontweight='bold', fontsize=12)
axs[1, 1].set_xlabel('Frequency (Hz)')
axs[1, 1].set_ylabel('Time (ms)')
axs[1, 1].grid(True, alpha=0.3)
axs[1, 1].set_xscale('log')

for freq, speed in zip(frequency_tests, frequency_speed):
    axs[1, 1].text(freq, speed, f'{speed:.2f}', ha='center', va='bottom', fontsize=8)

plt.tight_layout()
plt.savefig('output/speed_report.png', dpi=150, bbox_inches='tight')
plt.close()

fig2, axs2 = plt.subplots(2, 2, figsize=(16, 12))

for i, shape in enumerate(shapes):
    axs2[0, 0].plot(range(iterations), shape_times[shape], alpha=0.6,
                    linewidth=1, label=shape_names[i])

axs2[0, 0].set_title('Generation Time Distribution (50 Iterations)', fontweight='bold', fontsize=12)
axs2[0, 0].set_xlabel('Iteration')
axs2[0, 0].set_ylabel('Time (ms)')
axs2[0, 0].legend(loc='upper right', fontsize=8)
axs2[0, 0].grid(True, alpha=0.3)

for ftype in filter_types.keys():
    axs2[0, 1].plot(range(filter_iterations), filter_times[ftype],
                    alpha=0.7, linewidth=1.5, label=ftype.capitalize())

axs2[0, 1].set_title('Filter Processing Time Distribution', fontweight='bold', fontsize=12)
axs2[0, 1].set_xlabel('Iteration')
axs2[0, 1].set_ylabel('Time (ms)')
axs2[0, 1].legend(loc='upper right')
axs2[0, 1].grid(True, alpha=0.3)

all_shape_times = []
labels = []
for i, shape in enumerate(shapes):
    all_shape_times.append(shape_times[shape])
    labels.append(shape_names[i])

bp = axs2[1, 0].boxplot(all_shape_times, labels=labels, patch_artist=True,
                        notch=True, showmeans=True)

for patch, color in zip(bp['boxes'], colors):
    patch.set_facecolor(color)
    patch.set_alpha(0.7)

axs2[1, 0].set_title('Waveform Generation Time Distribution', fontweight='bold', fontsize=12)
axs2[1, 0].set_xlabel('Waveform Type')
axs2[1, 0].set_ylabel('Time (ms)')
axs2[1, 0].tick_params(axis='x', rotation=45)
axs2[1, 0].grid(True, alpha=0.3, axis='y')

all_filter_times = []
for ftype in filter_types.keys():
    all_filter_times.append(filter_times[ftype])

bp2 = axs2[1, 1].boxplot(all_filter_times, labels=filter_labels, patch_artist=True,
                         notch=True, showmeans=True)

for patch, color in zip(bp2['boxes'], filter_colors):
    patch.set_facecolor(color)
    patch.set_alpha(0.7)

axs2[1, 1].set_title('Filter Processing Time Distribution', fontweight='bold', fontsize=12)
axs2[1, 1].set_xlabel('Filter Type')
axs2[1, 1].set_ylabel('Time (ms)')
axs2[1, 1].tick_params(axis='x', rotation=45)
axs2[1, 1].grid(True, alpha=0.3, axis='y')

plt.tight_layout()
plt.savefig('output/speed_distribution.png', dpi=150, bbox_inches='tight')
plt.close()

throughput_samples = []
throughput_mbps = []

for duration_test in [1, 2, 5, 10, 15, 20]:
    times = []
    for _ in range(10):
        start = time.perf_counter()
        samples = resonix.generate_samples(resonix.Shape.SINE, duration_test, 440.0)
        end = time.perf_counter()
        times.append(end - start)
        del samples

    avg_time = np.mean(times)
    samples_per_sec = (duration_test * resonix.SAMPLE_RATE) / avg_time
    mb_per_sec = (samples_per_sec * 4) / (1024 * 1024)

    throughput_samples.append(samples_per_sec / 1e6)
    throughput_mbps.append(mb_per_sec)

fig3, axs3 = plt.subplots(1, 2, figsize=(16, 6))

axs3[0].plot([1, 2, 5, 10, 15, 20], throughput_samples, marker='o', color='forestgreen',
             linewidth=2.5, markersize=10, alpha=0.8)
axs3[0].set_title('Generation Throughput (Samples/sec)', fontweight='bold', fontsize=12)
axs3[0].set_xlabel('Duration (seconds)')
axs3[0].set_ylabel('Throughput (Million samples/sec)')
axs3[0].grid(True, alpha=0.3)

for x, y in zip([1, 2, 5, 10, 15, 20], throughput_samples):
    axs3[0].text(x, y, f'{y:.1f}M', ha='center', va='bottom', fontsize=9)

axs3[1].plot([1, 2, 5, 10, 15, 20], throughput_mbps, marker='s', color='darkviolet',
             linewidth=2.5, markersize=10, alpha=0.8)
axs3[1].set_title('Generation Throughput (MB/sec)', fontweight='bold', fontsize=12)
axs3[1].set_xlabel('Duration (seconds)')
axs3[1].set_ylabel('Throughput (MB/sec)')
axs3[1].grid(True, alpha=0.3)

for x, y in zip([1, 2, 5, 10, 15, 20], throughput_mbps):
    axs3[1].text(x, y, f'{y:.1f}', ha='center', va='bottom', fontsize=9)

plt.tight_layout()
plt.savefig('output/speed_throughput.png', dpi=150, bbox_inches='tight')

print("Test finished")