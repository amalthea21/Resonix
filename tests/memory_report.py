import resonix
import numpy as np
import os
import matplotlib.pyplot as plt

os.makedirs('output', exist_ok=True)

iterations = 100
memory_samples = []
allocation_sizes = []

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

shape_memory = {shape: [] for shape in shapes}

for i in range(iterations):
    for shape in shapes:
        samples = resonix.generate_samples(shape, 1, 440.0)
        memory_mb = samples.nbytes / (1024 * 1024)
        shape_memory[shape].append(memory_mb)
        del samples

filter_memory = []
filter_types = ['lowpass', 'highpass', 'bandpass', 'formant']
filter_data = {ftype: [] for ftype in filter_types}

for i in range(iterations):
    base_samples = resonix.generate_samples(resonix.Shape.SINE, 1, 440.0)

    lowpass = resonix.lowpass_filter(base_samples, 1000.0, 0.707)
    filter_data['lowpass'].append(lowpass.nbytes / (1024 * 1024))
    del lowpass

    highpass = resonix.highpass_filter(base_samples, 1000.0, 0.707)
    filter_data['highpass'].append(highpass.nbytes / (1024 * 1024))
    del highpass

    bandpass = resonix.bandpass_filter(base_samples, 1000.0, 500.0, 0.707)
    filter_data['bandpass'].append(bandpass.nbytes / (1024 * 1024))
    del bandpass

    formant = resonix.formant_filter(base_samples, 0.5, 0.8, 0.2)
    filter_data['formant'].append(formant.nbytes / (1024 * 1024))
    del formant

    del base_samples

duration_tests = [1, 2, 5, 10, 20]
duration_memory = []

for duration in duration_tests:
    samples = resonix.generate_samples(resonix.Shape.SINE, duration, 440.0)
    duration_memory.append(samples.nbytes / (1024 * 1024))
    del samples

fig, axs = plt.subplots(2, 2, figsize=(16, 12))

shape_names = ['SINE', 'SQUARE', 'TRIANGLE', 'SAWTOOTH', 'COSINE', 'TANGENT', 'COTANGENT', 'HANN', 'PHASED_HANN']
for i, shape in enumerate(shapes):
    axs[0, 0].plot(range(iterations), shape_memory[shape], alpha=0.7, label=shape_names[i], linewidth=1.5)

axs[0, 0].set_title('Memory Usage per Waveform (100 Iterations)', fontweight='bold', fontsize=12)
axs[0, 0].set_xlabel('Iteration')
axs[0, 0].set_ylabel('Memory (MB)')
axs[0, 0].legend(loc='upper right', fontsize=8)
axs[0, 0].grid(True, alpha=0.3)

for ftype in filter_types:
    axs[0, 1].plot(range(iterations), filter_data[ftype], alpha=0.7, label=ftype.capitalize(), linewidth=1.5)

axs[0, 1].set_title('Filter Memory Usage (100 Iterations)', fontweight='bold', fontsize=12)
axs[0, 1].set_xlabel('Iteration')
axs[0, 1].set_ylabel('Memory (MB)')
axs[0, 1].legend(loc='upper right')
axs[0, 1].grid(True, alpha=0.3)

axs[1, 0].bar(duration_tests, duration_memory, color='steelblue', alpha=0.7, edgecolor='black')
axs[1, 0].set_title('Memory Scaling vs Duration', fontweight='bold', fontsize=12)
axs[1, 0].set_xlabel('Duration (seconds)')
axs[1, 0].set_ylabel('Memory (MB)')
axs[1, 0].grid(True, alpha=0.3, axis='y')

for i, (duration, mem) in enumerate(zip(duration_tests, duration_memory)):
    axs[1, 0].text(duration, mem, f'{mem:.2f} MB', ha='center', va='bottom', fontsize=9)

avg_memories = []
shape_labels = []
for i, shape in enumerate(shapes):
    avg_mem = np.mean(shape_memory[shape])
    std_mem = np.std(shape_memory[shape])
    avg_memories.append(avg_mem)
    shape_labels.append(shape_names[i])

bars = axs[1, 1].bar(range(len(shapes)), avg_memories, color='coral', alpha=0.7, edgecolor='black')
axs[1, 1].set_title('Average Memory per Waveform Type', fontweight='bold', fontsize=12)
axs[1, 1].set_xlabel('Waveform Type')
axs[1, 1].set_ylabel('Average Memory (MB)')
axs[1, 1].set_xticks(range(len(shapes)))
axs[1, 1].set_xticklabels(shape_labels, rotation=45, ha='right', fontsize=8)
axs[1, 1].grid(True, alpha=0.3, axis='y')

for i, (bar, mem) in enumerate(zip(bars, avg_memories)):
    height = bar.get_height()
    axs[1, 1].text(bar.get_x() + bar.get_width() / 2., height,
                   f'{mem:.3f}', ha='center', va='bottom', fontsize=8)

plt.tight_layout()
plt.savefig('output/memory_report.png', dpi=150, bbox_inches='tight')
plt.close()

fig2, axs2 = plt.subplots(1, 2, figsize=(16, 6))

leak_test_iterations = 500
leak_memory = []

for i in range(leak_test_iterations):
    samples = resonix.generate_samples(resonix.Shape.SINE, 1, 440.0)
    leak_memory.append(samples.nbytes / (1024 * 1024))
    del samples

axs2[0].plot(range(leak_test_iterations), leak_memory, color='darkred', linewidth=1, alpha=0.8)
axs2[0].set_title('Memory Leak Detection (500 Iterations)', fontweight='bold', fontsize=12)
axs2[0].set_xlabel('Iteration')
axs2[0].set_ylabel('Memory (MB)')
axs2[0].grid(True, alpha=0.3)

mean_mem = np.mean(leak_memory)
std_mem = np.std(leak_memory)
axs2[0].axhline(y=mean_mem, color='green', linestyle='--', linewidth=2, label=f'Mean: {mean_mem:.3f} MB')
axs2[0].axhline(y=mean_mem + std_mem, color='orange', linestyle='--', linewidth=1.5, alpha=0.7,
                label=f'+1σ: {mean_mem + std_mem:.3f} MB')
axs2[0].axhline(y=mean_mem - std_mem, color='orange', linestyle='--', linewidth=1.5, alpha=0.7,
                label=f'-1σ: {mean_mem - std_mem:.3f} MB')
axs2[0].legend(loc='upper right')

all_filter_memory = []
for i in range(leak_test_iterations):
    base_samples = resonix.generate_samples(resonix.Shape.SINE, 1, 440.0)
    filtered = resonix.lowpass_filter(base_samples, 1000.0, 0.707)
    all_filter_memory.append(filtered.nbytes / (1024 * 1024))
    del filtered
    del base_samples

axs2[1].plot(range(leak_test_iterations), all_filter_memory, color='darkblue', linewidth=1, alpha=0.8)
axs2[1].set_title('Filter Memory Leak Detection (500 Iterations)', fontweight='bold', fontsize=12)
axs2[1].set_xlabel('Iteration')
axs2[1].set_ylabel('Memory (MB)')
axs2[1].grid(True, alpha=0.3)

mean_filter = np.mean(all_filter_memory)
std_filter = np.std(all_filter_memory)
axs2[1].axhline(y=mean_filter, color='green', linestyle='--', linewidth=2, label=f'Mean: {mean_filter:.3f} MB')
axs2[1].axhline(y=mean_filter + std_filter, color='orange', linestyle='--', linewidth=1.5, alpha=0.7,
                label=f'+1σ: {mean_filter + std_filter:.3f} MB')
axs2[1].axhline(y=mean_filter - std_filter, color='orange', linestyle='--', linewidth=1.5, alpha=0.7,
                label=f'-1σ: {mean_filter - std_filter:.3f} MB')
axs2[1].legend(loc='upper right')

plt.tight_layout()
plt.savefig('output/memory_report.png', dpi=150, bbox_inches='tight')

print("Test finished")