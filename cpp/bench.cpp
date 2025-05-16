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
// rev 7 [2023-10-16 15:36:16 +0300]: cache check
// rev 8 [2023-11-16 22:21:22 +0300]: cache check
// rev 9 [2023-12-24 22:59:35 +0300]: cache check
// rev 10 [2023-12-30 15:44:43 +0300]: cache check
// rev 11 [2024-01-21 21:57:53 +0300]: cache check
// rev 12 [2024-02-25 10:04:56 +0300]: cache check
// rev 13 [2024-03-05 22:02:40 +0300]: cache check
// rev 14 [2024-03-27 12:21:20 +0300]: cache check
// rev 15 [2024-04-12 14:12:36 +0300]: cache check
// rev 16 [2024-05-05 10:28:27 +0300]: cache check
// rev 17 [2024-05-28 15:30:02 +0300]: cache check
// rev 18 [2024-05-30 15:47:40 +0300]: cache check
// rev 19 [2024-06-12 11:14:08 +0300]: cache check
// rev 20 [2024-11-26 20:04:40 +0300]: cache check
// rev 21 [2025-01-08 18:08:45 +0300]: cache check
// rev 22 [2025-02-08 22:44:10 +0300]: cache check
// rev 23 [2025-03-09 09:04:12 +0300]: cache check
// rev 24 [2025-04-09 17:11:23 +0300]: cache check
// rev 25 [2025-05-16 11:39:49 +0300]: cache check
