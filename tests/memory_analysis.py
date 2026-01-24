import resonix
import numpy as np
import psutil
import os
import gc
import time
import tracemalloc
import matplotlib.pyplot as plt
from matplotlib.gridspec import GridSpec

process = psutil.Process(os.getpid())
tracemalloc.start()


def measure_memory_kb():
    gc.collect()
    time.sleep(0.05)
    return process.memory_info().rss / 1024


def measure_performance_precise(shape, duration, frequency, iterations=10):
    gc.collect()
    time.sleep(0.1)

    mem_before = measure_memory_kb()
    total_time = 0
    chunks = []

    for _ in range(iterations):
        gc.collect()
        time.sleep(0.02)

        start_time = time.perf_counter()
        chunk = resonix.generate_samples(shape, duration, frequency)
        total_time += (time.perf_counter() - start_time) * 1000
        chunks.append(chunk)

    del chunks
    gc.collect()
    time.sleep(0.1)
    mem_after = measure_memory_kb()

    avg_time = total_time / iterations
    mem_increase = mem_after - mem_before

    return avg_time, mem_increase


gc.collect()
time.sleep(0.3)
baseline_memory_mb = process.memory_info().rss / 1024 / 1024
baseline_memory_kb = baseline_memory_mb * 1024

chunks = []
chunk_numbers = []
memory_values_mb = []
memory_increases_kb = []
generation_times = []

for i in range(20):
    gc.collect()
    time.sleep(0.05)

    mem_before = measure_memory_kb()
    start_time = time.perf_counter()

    chunk = resonix.generate_samples(resonix.Shape.SINE, 60, 440.0)
    chunks.append(chunk)

    elapsed = (time.perf_counter() - start_time) * 1000
    mem_after = measure_memory_kb()
    mem_increase = mem_after - mem_before

    chunk_numbers.append(i + 1)
    memory_values_mb.append(mem_after / 1024)
    memory_increases_kb.append(mem_increase)
    generation_times.append(elapsed)

memory_before_cleanup = measure_memory_kb() / 1024

chunks.clear()
gc.collect()
time.sleep(0.5)

for _ in range(3):
    gc.collect()
    time.sleep(0.2)

memory_after_cleanup = measure_memory_kb() / 1024
leaked = memory_after_cleanup - baseline_memory_mb

shapes = [
    (resonix.Shape.SINE, 'SINE'),
    (resonix.Shape.SQUARE, 'SQUARE'),
    (resonix.Shape.SAWTOOTH, 'SAWTOOTH'),
    (resonix.Shape.TRIANGLE, 'TRIANGLE'),
    (resonix.Shape.PHASED_HANN, 'PHASED_HANN'),
    (resonix.Shape.HANN, 'HANN')
]

shape_names = []
shape_memory_kb = []
shape_times = []

for shape, name in shapes:
    time_ms, mem_increase_kb = measure_performance_precise(shape, 60, 440.0, iterations=10)
    shape_names.append(name)
    shape_memory_kb.append(mem_increase_kb)
    shape_times.append(time_ms)

durations = [1, 10, 30, 60, 120]
duration_memory_kb = []
duration_times = []
duration_theoretical_kb = []

SAMPLE_RATE = 44100
for duration in durations:
    time_ms, mem_increase_kb = measure_performance_precise(resonix.Shape.SINE, duration, 440.0)
    duration_memory_kb.append(mem_increase_kb)
    duration_times.append(time_ms)
    theoretical = (duration * SAMPLE_RATE * 8) / 1024
    duration_theoretical_kb.append(theoretical)

frequencies = [110.0, 220.0, 440.0, 880.0, 1760.0]
freq_memory_kb = []
freq_times = []

for freq in frequencies:
    time_ms, mem_increase_kb = measure_performance_precise(resonix.Shape.SINE, 60, freq)
    freq_memory_kb.append(mem_increase_kb)
    freq_times.append(time_ms)

current, peak = tracemalloc.get_traced_memory()
tracemalloc.stop()

fig = plt.figure(figsize=(18, 12))
gs = GridSpec(3, 3, figure=fig, hspace=0.35, wspace=0.35)

ax1 = fig.add_subplot(gs[0, :2])
ax1.plot(chunk_numbers, memory_values_mb, marker='o', linewidth=2, markersize=6,
         color='steelblue', label='Actual Memory')
ax1.axhline(y=baseline_memory_mb, color='red', linestyle='--',
            label=f'Baseline ({baseline_memory_mb:.1f} MB)', linewidth=2)
ax1.fill_between(chunk_numbers, baseline_memory_mb, memory_values_mb,
                 alpha=0.3, color='steelblue')
ax1.set_xlabel('Chunk Number', fontsize=12)
ax1.set_ylabel('Memory Usage (MB)', fontsize=12)
ax1.set_title('Test 1: Memory Growth (20× 60s SINE chunks)',
              fontsize=13, fontweight='bold', pad=10)
ax1.grid(True, alpha=0.3)
ax1.legend(fontsize=10)

ax2 = fig.add_subplot(gs[0, 2])
memory_increases_mb = [kb / 1024 for kb in memory_increases_kb]
colors = ['green' if x < 0.1 else 'orange' for x in memory_increases_mb]
ax2.bar(chunk_numbers, memory_increases_mb, color=colors, alpha=0.7, edgecolor='black', linewidth=0.5)
avg_increase_mb = np.mean(memory_increases_mb)
ax2.axhline(y=avg_increase_mb, color='red', linestyle='--',
            label=f'Avg: {avg_increase_mb:.3f} MB', linewidth=2)
ax2.set_xlabel('Chunk Number', fontsize=12)
ax2.set_ylabel('Memory Increase (MB)', fontsize=12)
ax2.set_title('Memory Increase Per Chunk', fontsize=13, fontweight='bold', pad=10)
ax2.grid(True, alpha=0.3, axis='y')
ax2.legend(fontsize=10)
ax2.set_ylim(0, max(memory_increases_mb) * 1.5 if memory_increases_mb else 1)

ax3 = fig.add_subplot(gs[1, 0])
ax3.plot(chunk_numbers, generation_times, marker='s', linewidth=2,
         markersize=5, color='green', label='Generation Time')
avg_time = np.mean(generation_times)
ax3.axhline(y=avg_time, color='red', linestyle='--',
            label=f'Avg: {avg_time:.1f} ms', linewidth=2)
ax3.set_xlabel('Chunk Number', fontsize=12)
ax3.set_ylabel('Generation Time (ms)', fontsize=12)
ax3.set_title('Generation Performance', fontsize=13, fontweight='bold', pad=10)
ax3.grid(True, alpha=0.3)
ax3.legend(fontsize=10)

ax4 = fig.add_subplot(gs[1, 1])
shape_memory_mb = [kb / 1024 for kb in shape_memory_kb]
colors_wave = ['#1f77b4', '#ff7f0e', '#2ca02c', '#d62728', '#9467bd', '#8c564b']
bars = ax4.bar(range(len(shape_names)), shape_memory_mb,
               color=colors_wave, alpha=0.7, edgecolor='black', linewidth=0.5)
ax4.set_xticks(range(len(shape_names)))
ax4.set_xticklabels(shape_names, rotation=0, ha='center', fontsize=11)
ax4.set_ylabel('Memory Increase (MB)', fontsize=12)
ax4.set_title('Test 3: Memory by Waveform (60s, 440Hz)', fontsize=13, fontweight='bold', pad=10)
ax4.grid(True, alpha=0.3, axis='y')
if shape_memory_mb:
    ax4.set_ylim(0, max(shape_memory_mb) * 1.3)

for i, (bar, val) in enumerate(zip(bars, shape_memory_mb)):
    height = bar.get_height()
    ax4.text(bar.get_x() + bar.get_width() / 2., height,
             f'{val:.3f}', ha='center', va='bottom', fontsize=9)

ax5 = fig.add_subplot(gs[1, 2])
bars = ax5.bar(range(len(shape_names)), shape_times,
               color=colors_wave, alpha=0.7, edgecolor='black', linewidth=0.5)
ax5.set_xticks(range(len(shape_names)))
ax5.set_xticklabels(shape_names, rotation=0, ha='center', fontsize=11)
ax5.set_ylabel('Generation Time (ms)', fontsize=12)
ax5.set_title('Performance by Waveform (60s, 440Hz)', fontsize=13, fontweight='bold', pad=10)
ax5.grid(True, alpha=0.3, axis='y')

for bar, val in zip(bars, shape_times):
    height = bar.get_height()
    ax5.text(bar.get_x() + bar.get_width() / 2., height,
             f'{val:.1f}', ha='center', va='bottom', fontsize=9)

ax6 = fig.add_subplot(gs[2, 0])
duration_memory_mb = [kb / 1024 for kb in duration_memory_kb]
duration_theoretical_mb = [kb / 1024 for kb in duration_theoretical_kb]

ax6.plot(durations, duration_memory_mb, marker='o', linewidth=2.5,
         markersize=8, color='purple', label='Measured')
ax6.plot(durations, duration_theoretical_mb, linestyle='--', linewidth=2,
         color='orange', label='Theoretical', alpha=0.7)

ax6.set_xlabel('Duration (seconds)', fontsize=12)
ax6.set_ylabel('Memory Increase (MB)', fontsize=12)
ax6.set_title('Test 4: Memory Scaling by Duration (SINE, 440Hz)', fontsize=13, fontweight='bold', pad=10)
ax6.grid(True, alpha=0.3)
ax6.legend(fontsize=10)

for i, (actual, theoretical) in enumerate(zip(duration_memory_mb, duration_theoretical_mb)):
    ratio = actual / theoretical if theoretical > 0 else 0
    ax6.text(durations[i], actual, f'  {ratio:.1%}', va='bottom', fontsize=8)

ax7 = fig.add_subplot(gs[2, 1])
freq_memory_mb = [kb / 1024 for kb in freq_memory_kb]
freq_labels = [f'{int(f)}' for f in frequencies]
bars = ax7.bar(range(len(freq_labels)), freq_memory_mb,
               color='orange', alpha=0.7, edgecolor='black', linewidth=0.5)
ax7.set_xticks(range(len(freq_labels)))
ax7.set_xticklabels(freq_labels, rotation=0, fontsize=11)
ax7.set_xlabel('Frequency (Hz)', fontsize=12)
ax7.set_ylabel('Memory Increase (MB)', fontsize=12)
ax7.set_title('Test 5: Memory vs Frequency (SINE, 60s)', fontsize=13, fontweight='bold', pad=10)
ax7.grid(True, alpha=0.3, axis='y')

if freq_memory_mb:
    if max(freq_memory_mb) - min(freq_memory_mb) < 0.01:
        avg_mem = np.mean(freq_memory_mb)
        ax7.set_ylim(avg_mem - 0.05, avg_mem + 0.05)
    else:
        ax7.set_ylim(0, max(freq_memory_mb) * 1.2)

for bar, val in zip(bars, freq_memory_mb):
    height = bar.get_height()
    ax7.text(bar.get_x() + bar.get_width() / 2., height,
             f'{val:.3f}', ha='center', va='bottom', fontsize=8)

ax8 = fig.add_subplot(gs[2, 2])
leak_data = [baseline_memory_mb, memory_before_cleanup, memory_after_cleanup]
leak_labels = ['Baseline', 'Before\nCleanup', 'After\nCleanup']

if leaked < 5:
    colors_leak = ['green', 'red', 'green']
    leak_status = "✓ No leak"
    status_color = 'lightgreen'
elif leaked < 20:
    colors_leak = ['green', 'red', 'orange']
    leak_status = "⚠ Minor leak"
    status_color = 'wheat'
else:
    colors_leak = ['green', 'red', 'red']
    leak_status = "✗ Leak detected"
    status_color = 'lightcoral'

bars = ax8.bar(range(len(leak_labels)), leak_data,
               color=colors_leak, alpha=0.7, edgecolor='black', linewidth=1)
ax8.set_xticks(range(len(leak_labels)))
ax8.set_xticklabels(leak_labels, fontsize=11)
ax8.set_ylabel('Memory (MB)', fontsize=12)
ax8.set_title('Test 2: Memory Leak Detection', fontsize=13, fontweight='bold', pad=10)
ax8.grid(True, alpha=0.3, axis='y')

for bar, val in zip(bars, leak_data):
    height = bar.get_height()
    ax8.text(bar.get_x() + bar.get_width() / 2., height,
             f'{val:.1f}', ha='center', va='bottom', fontsize=9)

ax8.text(0.5, 0.95, f'{leak_status}\n({leaked:.1f} MB leaked)',
         transform=ax8.transAxes, ha='center', va='top',
         bbox=dict(boxstyle='round', facecolor=status_color, alpha=0.8, edgecolor='black', linewidth=1.5),
         fontsize=11, fontweight='bold')

plt.savefig('output/memory_analysis.png', dpi=150, bbox_inches='tight')
plt.show()

print('Test finished')