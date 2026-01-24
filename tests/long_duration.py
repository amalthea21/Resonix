import resonix
import numpy as np
import psutil
import os
import matplotlib.pyplot as plt

process = psutil.Process(os.getpid())

chunks = []
baseline_memory = process.memory_info().rss / 1024 / 1024  # MB

chunk_numbers = []
memory_values = []
memory_increases = []

for i in range(20):
    print(f"Generating chunk {i + 1}/20...")

    mem_before = process.memory_info().rss / 1024 / 1024

    chunk = resonix.generate_samples(resonix.Shape.SINE, 60, 440.0)
    chunks.append(chunk)

    mem_after = process.memory_info().rss / 1024 / 1024
    mem_increase = mem_after - mem_before

    chunk_numbers.append(i + 1)
    memory_values.append(mem_after)
    memory_increases.append(mem_increase)

    print(f"  Memory: +{mem_increase:.2f} MB (Total: {mem_after:.2f} MB)")

print(f"\nBaseline memory: {baseline_memory:.2f} MB")
print(f"Final memory: {process.memory_info().rss / 1024 / 1024:.2f} MB")
print(f"Total increase: {(process.memory_info().rss / 1024 / 1024) - baseline_memory:.2f} MB")
print(f"Total signal length: {len(np.concatenate(chunks))} samples ({len(np.concatenate(chunks)) / resonix.SAMPLE_RATE:.1f} seconds)")

fig, (ax1, ax2) = plt.subplots(2, 1, figsize=(10, 8))

ax1.plot(chunk_numbers, memory_values, marker='o', linewidth=2, markersize=6)
ax1.axhline(y=baseline_memory, color='r', linestyle='--', label='Baseline')
ax1.fill_between(chunk_numbers, baseline_memory, memory_values, alpha=0.3)
ax1.set_xlabel('Chunk Number')
ax1.set_ylabel('Memory Usage (MB)')
ax1.set_title('Total Memory Usage Over Time')
ax1.grid(True, alpha=0.3)
ax1.legend()

ax2.bar(chunk_numbers, memory_increases, color='steelblue', alpha=0.7)
ax2.axhline(y=np.mean(memory_increases), color='r', linestyle='--',
            label=f'Average: {np.mean(memory_increases):.2f} MB')
ax2.set_xlabel('Chunk Number')
ax2.set_ylabel('Memory Increase (MB)')
ax2.set_title('Memory Increase Per Chunk')
ax2.grid(True, alpha=0.3, axis='y')
ax2.legend()

plt.tight_layout()
plt.savefig('output/long_duration.png', dpi=150, bbox_inches='tight')