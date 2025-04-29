#include <iostream>
#include <fstream>
#include <vector>
#include <thread>
#include <atomic>
#include <zlib.h>
#include <cstdint>

#include "threads_benchmark.hpp"
#include "worker.hpp"

const std::string APPEND_TEXT = "He-he-he";

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

    // std::vector<uint8_t> original_data;
    // file.seekg(0, std::ios::end);
    // std::streamsize size = file.tellg();
    // file.seekg(0, std::ios::beg);
    // original_data.resize(size);
    // file.read(reinterpret_cast<char*>(original_data.data()), size);

    original_data.insert(original_data.end(), APPEND_TEXT.begin(), APPEND_TEXT.end());

    uint32_t original_crc = calculate_crc32(original_data);

    unsigned int t = benchmark_threads(original_data); 

    // number of possible combinations for 4 byte  
    uint64_t total = uint64_t(1) << 32;
    uint64_t chunk = total / t;

    std::vector<std::thread> threads;
    std::vector<std::vector<uint8_t>> results(t);  // каждый поток пишет в свой result

    for (unsigned int i = 0; i < t; ++i) {
        auto start = i * chunk;
        auto end = (i == t - 1) ? total : (start + chunk);

        threads.emplace_back(worker, std::cref(original_data), 
                    original_crc, start, end, &results[i]);
    }

    for (auto& t : threads) {
        t.join();
    }

    // Поиск успешного результата
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
