#include "threads_benchmark.hpp"
#include <thread>
#include <atomic>
#include <iostream>
#include <zlib.h>
#include <chrono>

int benchmark_threads(const std::vector<uint8_t>& base, uint32_t iterations) {
    std::vector<int> thread_counts = {1, 2, 4, 8, 16, 32};
    uint32_t dummy_target = 0xDEADBEEF;

    int best_thread_count = 1;
    double best_speed = 0.0;

    for (int t : thread_counts) {
        if (t > std::thread::hardware_concurrency()) break;

        std::vector<std::thread> threads;

        uint64_t chunk = iterations / t;

        auto start_time = std::chrono::high_resolution_clock::now();

        for (int i = 0; i < t; ++i) {
            uint32_t start = i * chunk;
            uint32_t end = (i == t - 1) ? iterations : (start + chunk);

            threads.emplace_back([&, start, end]() {
                std::vector<uint8_t> suffix(4);
                uint32_t prefix_crc = crc32(0, base.data(), base.size());

                for (uint32_t val = start; val < end; ++val) {
                    suffix[0] = (val >> 24) & 0xFF;
                    suffix[1] = (val >> 16) & 0xFF;
                    suffix[2] = (val >> 8) & 0xFF;
                    suffix[3] = val & 0xFF;

                    crc32(prefix_crc, suffix.data(), 4); // просто считаем
                }
            });
        }

        for (auto& th : threads) th.join();

        auto end_time = std::chrono::high_resolution_clock::now();
        auto duration_sec = std::chrono::duration<double>(end_time - start_time).count();

        double speed = iterations / duration_sec;

        std::cout << "[Benchmark] Threads: " << t << ", Time: " << duration_sec << "s, Speed: " << speed << " hashes/s\n";

        if (speed > best_speed) {
            best_speed = speed;
            best_thread_count = t;
        }
    }

    std::cout << "[Benchmark] Best thread count: " << best_thread_count << "\n";

    return best_thread_count;
}
