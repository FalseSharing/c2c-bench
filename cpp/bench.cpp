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
// rev 1 [2022-12-16 16:46:18 +0300]: cache check
// rev 2 [2022-12-21 15:40:18 +0300]: cache check
// rev 3 [2023-03-04 13:53:46 +0300]: cache check
// rev 4 [2023-05-23 21:48:35 +0300]: cache check
// rev 5 [2023-07-07 16:35:52 +0300]: cache check
// rev 6 [2023-08-29 14:41:09 +0300]: cache check
