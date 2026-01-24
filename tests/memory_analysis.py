import resonix
import numpy as np
import psutil
import os
import gc
import time
import tracemalloc
import matplotlib.pyplot as plt
from matplotlib.gridspec import GridSpec

os.makedirs('output', exist_ok=True)

gc.set_threshold(700, 10, 10)

process = psutil.Process(os.getpid())
tracemalloc.start()


def measure_memory_kb():
    gc.collect()
    time.sleep(0.1)
    return process.memory_info().rss / 1024


def measure_performance_precise(shape, duration, frequency, iterations=10):
    gc.collect()
    time.sleep(0.2)

    mem_before = measure_memory_kb()
    total_time = 0
    chunks = []

    for _ in range(iterations):
        gc.collect()
        time.sleep(0.05)

        start_time = time.perf_counter()
        chunk = resonix.generate_samples(shape, duration, frequency)
        total_time += (time.perf_counter() - start_time) * 1000
        chunks.append(chunk)

    del chunks
    gc.collect()
    time.sleep(0.2)
    mem_after = measure_memory_kb()

    avg_time = total_time / iterations
    mem_increase = mem_after - mem_before

    return avg_time, mem_increase


gc.collect()
time.sleep(0.5)
baseline_memory_mb = process.memory_info().rss / 1024 / 1024
baseline_memory_kb = baseline_memory_mb * 1024

leak_test_chunks = []
leak_test_memory_mb = []
leak_test_chunk_nums = []

for leak_cycle in range(3):
    cycle_memory = []

    for i in range(10):
        gc.collect()
        time.sleep(0.1)

        mem_before = measure_memory_kb()
        chunk = resonix.generate_samples(resonix.Shape.SINE, 5, 440.0)
        leak_test_chunks.append(chunk)

        mem_after = measure_memory_kb()
        cycle_memory.append(mem_after / 1024)

    leak_test_memory_mb.extend(cycle_memory)
    leak_test_chunk_nums.extend(range(leak_cycle * 10, (leak_cycle + 1) * 10))

    if len(leak_test_chunks) > 15:
        del leak_test_chunks[:5]
        gc.collect()
        time.sleep(0.2)

del leak_test_chunks
gc.collect()
time.sleep(1.0)

chunks = []
chunk_numbers = []
memory_values_mb = []
memory_increases_kb = []
generation_times = []

for i in range(20):
    gc.collect()
    time.sleep(0.1)

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
del chunks
gc.collect()
plt.close('all')
time.sleep(2.0)

for _ in range(5):
    gc.collect()
    time.sleep(0.5)

memory_after_cleanup = measure_memory_kb() / 1024
leaked = memory_after_cleanup - baseline_memory_mb

shapes = [
    (resonix.Shape.SINE, 'SINE'),
    (resonix.Shape.COSINE, 'COSINE'),
    (resonix.Shape.TANGENT, 'TANGENT'),
    (resonix.Shape.COTANGENT, 'COTANGENT'),
    (resonix.Shape.SQUARE, 'SQUARE'),
    (resonix.Shape.SAWTOOTH, 'SAWTOOTH'),
    (resonix.Shape.TRIANGLE, 'TRIANGLE'),
    (resonix.Shape.PHASED_HANN, 'PHASED_HANN'),
    (resonix.Shape.HANN, 'HANN'),
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
gs = GridSpec(2, 3, figure=fig, hspace=0.35, wspace=0.35)

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

ax1b = fig.add_subplot(gs[0, 2])
ax1b.plot(leak_test_chunk_nums, leak_test_memory_mb, marker='o', linewidth=2, markersize=5,
          color='purple', label='Memory per Chunk')
ax1b.axhline(y=baseline_memory_mb, color='red', linestyle='--',
             label=f'Baseline', linewidth=2)
ax1b.set_xlabel('Chunk Number (3 cycles)', fontsize=12)
ax1b.set_ylabel('Memory Usage (MB)', fontsize=12)
ax1b.set_title('Memory Leak Test (3×10 chunks with partial cleanup)',
               fontsize=13, fontweight='bold', pad=10)
ax1b.grid(True, alpha=0.3)
ax1b.legend(fontsize=10)

leak_start = leak_test_memory_mb[0]
leak_end = leak_test_memory_mb[-1]
leak_increase = leak_end - leak_start
leak_per_chunk = leak_increase / len(leak_test_chunk_nums) if len(leak_test_chunk_nums) > 0 else 0

leak_text = f'Leak Analysis:\nStart: {leak_start:.1f} MB\nEnd: {leak_end:.1f} MB\nIncrease: {leak_increase:.2f} MB\nPer Chunk: {leak_per_chunk:.3f} MB'
props = dict(boxstyle='round', facecolor='wheat', alpha=0.8)
ax1b.text(0.05, 0.95, leak_text, transform=ax1b.transAxes, fontsize=9,
          verticalalignment='top', bbox=props)

ax2 = fig.add_subplot(gs[1, 0])
ax2.plot(chunk_numbers, generation_times, marker='s', linewidth=2,
         markersize=5, color='green', label='Generation Time')
avg_time = np.mean(generation_times)
ax2.axhline(y=avg_time, color='red', linestyle='--',
            label=f'Avg: {avg_time:.1f} ms', linewidth=2)
ax2.set_xlabel('Chunk Number', fontsize=12)
ax2.set_ylabel('Generation Time (ms)', fontsize=12)
ax2.set_title('Generation Performance', fontsize=13, fontweight='bold', pad=10)
ax2.grid(True, alpha=0.3)
ax2.legend(fontsize=10)

ax3 = fig.add_subplot(gs[1, 1:])
colors_wave = ['#1f77b4', '#ff7f0e', '#2ca02c', '#d62728', '#9467bd', '#8c564b', '#e377c2', '#7f7f7f', '#bcbd22']
bars = ax3.bar(range(len(shape_names)), shape_times,
               color=colors_wave, alpha=0.7, edgecolor='black', linewidth=0.5)
ax3.set_xticks(range(len(shape_names)))
ax3.set_xticklabels(shape_names, rotation=45, ha='right', fontsize=11)
ax3.set_ylabel('Generation Time (ms)', fontsize=12)
ax3.set_title('Performance by Waveform (60s, 440Hz)', fontsize=13, fontweight='bold', pad=10)
ax3.grid(True, alpha=0.3, axis='y')

for bar, val in zip(bars, shape_times):
    height = bar.get_height()
    ax3.text(bar.get_x() + bar.get_width() / 2., height,
             f'{val:.1f}', ha='center', va='bottom', fontsize=9)

plt.savefig('output/memory_analysis.png', dpi=150, bbox_inches='tight')
plt.close('all')

print('Test finished')