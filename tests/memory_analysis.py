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

gc.collect()
time.sleep(0.5)
baseline_memory = process.memory_info().rss / 1024 / 1024

chunks = []
chunk_numbers = []
memory_values = []
memory_increases = []
generation_times = []

for i in range(20):
    gc.collect()
    mem_before = process.memory_info().rss / 1024 / 1024
    start_time = time.time()

    chunk = resonix.generate_samples(resonix.Shape.SINE, 60, 440.0)
    chunks.append(chunk)

    elapsed = time.time() - start_time
    mem_after = process.memory_info().rss / 1024 / 1024
    mem_increase = mem_after - mem_before

    chunk_numbers.append(i + 1)
    memory_values.append(mem_after)
    memory_increases.append(mem_increase)
    generation_times.append(elapsed * 1000)

total_samples = len(np.concatenate(chunks))
total_duration = total_samples / resonix.SAMPLE_RATE

memory_before_cleanup = process.memory_info().rss / 1024 / 1024

chunks.clear()
gc.collect()
time.sleep(0.5)

memory_after_first_gc = process.memory_info().rss / 1024 / 1024

for _ in range(3):
    gc.collect()
    time.sleep(0.2)

memory_after_cleanup = process.memory_info().rss / 1024 / 1024

leaked = memory_after_cleanup - baseline_memory

shapes = [
    (resonix.Shape.SINE, 'SINE'),
    (resonix.Shape.SQUARE, 'SQUARE'),
    (resonix.Shape.SAWTOOTH, 'SAWTOOTH'),
    (resonix.Shape.TRIANGLE, 'TRIANGLE')
]
shape_names = []
shape_memory = []
shape_times = []

for shape, name in shapes:
    gc.collect()
    time.sleep(0.2)

    mem_before = process.memory_info().rss / 1024 / 1024
    start_time = time.time()

    test_chunk = resonix.generate_samples(shape, 60, 440.0)

    elapsed = time.time() - start_time
    mem_after = process.memory_info().rss / 1024 / 1024
    mem_increase = mem_after - mem_before

    shape_names.append(name)
    shape_memory.append(mem_increase)
    shape_times.append(elapsed * 1000)

    del test_chunk
    gc.collect()

durations = [1, 10, 30, 60, 120]
duration_memory = []
duration_times = []

for duration in durations:
    gc.collect()
    time.sleep(0.2)

    mem_before = process.memory_info().rss / 1024 / 1024
    start_time = time.time()

    test_chunk = resonix.generate_samples(resonix.Shape.SINE, duration, 440.0)

    elapsed = time.time() - start_time
    mem_after = process.memory_info().rss / 1024 / 1024
    mem_increase = mem_after - mem_before

    duration_memory.append(mem_increase)
    duration_times.append(elapsed * 1000)

    del test_chunk
    gc.collect()

frequencies = [110.0, 220.0, 440.0, 880.0, 1760.0]
freq_memory = []
freq_times = []

for freq in frequencies:
    gc.collect()
    time.sleep(0.2)

    mem_before = process.memory_info().rss / 1024 / 1024
    start_time = time.time()

    test_chunk = resonix.generate_samples(resonix.Shape.SINE, 60, freq)

    elapsed = time.time() - start_time
    mem_after = process.memory_info().rss / 1024 / 1024
    mem_increase = mem_after - mem_before

    freq_memory.append(mem_increase)
    freq_times.append(elapsed * 1000)

    del test_chunk
    gc.collect()

current, peak = tracemalloc.get_traced_memory()
tracemalloc.stop()

fig = plt.figure(figsize=(18, 12))
gs = GridSpec(3, 3, figure=fig, hspace=0.35, wspace=0.35)

ax1 = fig.add_subplot(gs[0, :2])
ax1.plot(chunk_numbers, memory_values, marker='o', linewidth=2, markersize=6,
         color='steelblue', label='Actual Memory')
ax1.axhline(y=baseline_memory, color='red', linestyle='--',
            label=f'Baseline ({baseline_memory:.1f} MB)', linewidth=2)
ax1.fill_between(chunk_numbers, baseline_memory, memory_values,
                 alpha=0.3, color='steelblue')
ax1.set_xlabel('Chunk Number', fontsize=12)
ax1.set_ylabel('Memory Usage (MB)', fontsize=12)
ax1.set_title('Test 1: Memory Growth (20× 60s SINE chunks)',
              fontsize=13, fontweight='bold', pad=10)
ax1.grid(True, alpha=0.3)
ax1.legend(fontsize=10)

ax2 = fig.add_subplot(gs[0, 2])
colors = ['green' if x < 11 else 'orange' for x in memory_increases]
ax2.bar(chunk_numbers, memory_increases, color=colors, alpha=0.7, edgecolor='black', linewidth=0.5)
avg_increase = np.mean(memory_increases)
ax2.axhline(y=avg_increase, color='red', linestyle='--',
            label=f'Avg: {avg_increase:.2f} MB', linewidth=2)
ax2.set_xlabel('Chunk Number', fontsize=12)
ax2.set_ylabel('Memory Increase (MB)', fontsize=12)
ax2.set_title('Memory Increase Per Chunk', fontsize=13, fontweight='bold', pad=10)
ax2.grid(True, alpha=0.3, axis='y')
ax2.legend(fontsize=10)
ax2.set_ylim(0, max(memory_increases) * 1.2)

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
colors_wave = ['#1f77b4', '#ff7f0e', '#2ca02c', '#d62728']
bars = ax4.bar(range(len(shape_names)), shape_memory,
               color=colors_wave, alpha=0.7, edgecolor='black', linewidth=0.5)
ax4.set_xticks(range(len(shape_names)))
ax4.set_xticklabels(shape_names, rotation=0, ha='center', fontsize=11)
ax4.set_ylabel('Memory Increase (MB)', fontsize=12)
ax4.set_title('Test 3: Memory by Waveform', fontsize=13, fontweight='bold', pad=10)
ax4.grid(True, alpha=0.3, axis='y')
ax4.set_ylim(0, max(shape_memory) * 1.2)

for i, (bar, val) in enumerate(zip(bars, shape_memory)):
    height = bar.get_height()
    ax4.text(bar.get_x() + bar.get_width() / 2., height,
             f'{val:.2f}', ha='center', va='bottom', fontsize=9)

ax5 = fig.add_subplot(gs[1, 2])
bars = ax5.bar(range(len(shape_names)), shape_times,
               color=colors_wave, alpha=0.7, edgecolor='black', linewidth=0.5)
ax5.set_xticks(range(len(shape_names)))
ax5.set_xticklabels(shape_names, rotation=0, ha='center', fontsize=11)
ax5.set_ylabel('Generation Time (ms)', fontsize=12)
ax5.set_title('Performance by Waveform', fontsize=13, fontweight='bold', pad=10)
ax5.grid(True, alpha=0.3, axis='y')

for bar, val in zip(bars, shape_times):
    height = bar.get_height()
    ax5.text(bar.get_x() + bar.get_width() / 2., height,
             f'{val:.1f}', ha='center', va='bottom', fontsize=9)

ax6 = fig.add_subplot(gs[2, 0])
ax6.plot(durations, duration_memory, marker='o', linewidth=2.5,
         markersize=8, color='purple', label='Measured')

theoretical = [(d * resonix.SAMPLE_RATE * 4) / (1024 * 1024) for d in durations]
ax6.plot(durations, theoretical, linestyle='--', linewidth=2,
         color='orange', label='Theoretical', alpha=0.7)

ax6.set_xlabel('Duration (seconds)', fontsize=12)
ax6.set_ylabel('Memory Increase (MB)', fontsize=12)
ax6.set_title('Test 4: Memory Scaling by Duration', fontsize=13, fontweight='bold', pad=10)
ax6.grid(True, alpha=0.3)
ax6.legend(fontsize=10)

ax7 = fig.add_subplot(gs[2, 1])
freq_labels = [f'{int(f)}' for f in frequencies]
bars = ax7.bar(range(len(freq_labels)), freq_memory,
               color='orange', alpha=0.7, edgecolor='black', linewidth=0.5)
ax7.set_xticks(range(len(freq_labels)))
ax7.set_xticklabels(freq_labels, rotation=0, fontsize=11)
ax7.set_xlabel('Frequency (Hz)', fontsize=12)
ax7.set_ylabel('Memory Increase (MB)', fontsize=12)
ax7.set_title('Test 5: Memory vs Frequency', fontsize=13, fontweight='bold', pad=10)
ax7.grid(True, alpha=0.3, axis='y')

if max(freq_memory) - min(freq_memory) < 1:
    avg_mem = np.mean(freq_memory)
    ax7.set_ylim(avg_mem - 1, avg_mem + 1)
else:
    ax7.set_ylim(0, max(freq_memory) * 1.2)

for bar, val in zip(bars, freq_memory):
    height = bar.get_height()
    ax7.text(bar.get_x() + bar.get_width() / 2., height,
             f'{val:.3f}', ha='center', va='bottom', fontsize=8)

ax8 = fig.add_subplot(gs[2, 2])
leak_data = [baseline_memory, memory_before_cleanup, memory_after_cleanup]
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