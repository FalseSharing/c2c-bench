# c2c-bench

Micro-benchmarks evaluating cacheline contention (false sharing) and cache-to-cache (c2c) transfer latency on multi-core x86_64 architectures.

### Overview

When threads on different processor cores write to distinct variables located on the same 64-byte cache line, the MESI/MOESI cache coherency protocol forces constant line invalidations (HITM - Hit Modified transfers). This project benchmarks the latency differential and provides automated `perf c2c` profiling wrappers.

### Components

* `cpp/`: C++20 pthreads/std::jthread benchmarks comparing contiguous shared state vs `alignas(64)` padded state.
* `go/`: Go runtime benchmarks evaluating `sync/atomic` operations with and without CPU cache padding.
* `perf/`: Shell and Python scripts parsing `perf c2c` metrics and calculating HITM penalties.

### Quick Start

```bash
# Build C++ benchmarks
cd cpp && make
./bench_unpadded
./bench_padded

# Run Go benchmarks
cd ../go
go test -bench=. -benchmem

# Record perf c2c trace (Linux)
sudo ../perf/record_c2c.sh
python3 ../perf/analyze.py
```
