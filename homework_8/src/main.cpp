#include <iostream>
#include <fstream>
#include <vector>
#include <thread>
#include <atomic>
#include <cstdint>
#include <zlib.h>

#include "threads_benchmark.hpp"
#include "worker.hpp"

const std::string APPEND_TEXT = "He-he-he";

uint32_t calculate_crc32(const std::vector<uint8_t> &data)
{
    return crc32(0, data.data(), data.size());
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr << "Usage: ./crc32_crack <input_file> <output_file>\n";
        return 1;
    }

    std::string input_path = argv[1];
    std::string output_path = argv[2];

    std::ifstream file(input_path, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Cannot open input file.\n";
        return 1;
    }

    std::vector<uint8_t> original_data((std::istreambuf_iterator<char>(file)),
                                        std::istreambuf_iterator<char>());

    // Вычисляем CRC32 от оригинальных данных (до добавления текста)
    auto original_crc = calculate_crc32(original_data);

    // Готовим данные, к которым будем добавлять 4 байта — это оригинал + APPEND_TEXT
    std::vector<uint8_t> base = original_data;
    base.insert(base.end(), APPEND_TEXT.begin(), APPEND_TEXT.end());

    // Количество потоков
    auto t = benchmark_threads(base);

    // Подготовка диапазонов
    uint64_t total = uint64_t(1) << 32;
    uint64_t chunk = total / t;

    std::vector<std::thread> threads;
    std::vector<std::vector<uint8_t>> results(t);  // Каждый поток записывает сюда

    for (unsigned int i = 0; i < t; ++i) {
        auto start = i * chunk;
        auto end = (i == t - 1) ? total : (start + chunk);

        threads.emplace_back(worker, base, original_crc, start, end, &results[i]);
    }

    for (auto& t : threads) {
        t.join();
    }

    for (const auto& result : results) {
        if (!result.empty()) {
            std::ofstream out(output_path, std::ios::binary);
            out.write(reinterpret_cast<const char*>(result.data()), result.size());

            std::cout << "Success! Result written to " << output_path << "\n";
            return 0;
        }
    }

    std::cerr << "No matching 4 bytes found.\n";
    return 1;
}
