import resonix
import numpy as np
import psutil
import os

# Get current process
process = psutil.Process(os.getpid())

chunks = []
baseline_memory = process.memory_info().rss / 1024 / 1024  # MB

for i in range(20):
    print(f"Generating chunk {i + 1}/20...")

    # Memory before
    mem_before = process.memory_info().rss / 1024 / 1024

    # Generate 1-minute chunk
    chunk = resonix.generate_samples(resonix.Shape.SINE, 60, 440.0)
    chunks.append(chunk)

    # Memory after
    mem_after = process.memory_info().rss / 1024 / 1024
    mem_increase = mem_after - mem_before

    print(f"  Memory: +{mem_increase:.2f} MB (Total: {mem_after:.2f} MB)")

print(f"\nBaseline memory: {baseline_memory:.2f} MB")
print(f"Final memory: {process.memory_info().rss / 1024 / 1024:.2f} MB")
print(f"Total increase: {(process.memory_info().rss / 1024 / 1024) - baseline_memory:.2f} MB")
print(
    f"Total signal length: {len(np.concatenate(chunks))} samples ({len(np.concatenate(chunks)) / resonix.SAMPLE_RATE:.1f} seconds)")