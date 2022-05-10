#include <atomic>
#include <chrono>
#include <iostream>
#include <thread>
#include <vector>

constexpr size_t ITERATIONS = 100'000'000;
constexpr size_t THREADS = 4;

struct UnpaddedState {
    uint64_t values[THREADS];
};

struct alignas(64) PaddedSlot {
    uint64_t val;
    char pad[64 - sizeof(uint64_t)];
};

struct PaddedState {
    PaddedSlot slots[THREADS];
};

void run_unpadded() {
    UnpaddedState state{};
    std::vector<std::thread> workers;
    auto start = std::chrono::high_resolution_clock::now();

    for (size_t i = 0; i < THREADS; ++i) {
        workers.emplace_back([&state, i]() {
            for (size_t j = 0; j < ITERATIONS; ++j) {
                state.values[i]++;
            }
        });
    }
    for (auto& t : workers) t.join();

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> elapsed = end - start;
    std::cout << "[Unpadded] Elapsed: " << elapsed.count() << " ms\n";
}

void run_padded() {
    PaddedState state{};
    std::vector<std::thread> workers;
    auto start = std::chrono::high_resolution_clock::now();

    for (size_t i = 0; i < THREADS; ++i) {
        workers.emplace_back([&state, i]() {
            for (size_t j = 0; j < ITERATIONS; ++j) {
                state.slots[i].val++;
            }
        });
    }
    for (auto& t : workers) t.join();

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> elapsed = end - start;
    std::cout << "[Padded]   Elapsed: " << elapsed.count() << " ms\n";
}

int main() {
    std::cout << "Testing false sharing across " << THREADS << " hardware cores...\n";
    run_unpadded();
    run_padded();
    return 0;
}
