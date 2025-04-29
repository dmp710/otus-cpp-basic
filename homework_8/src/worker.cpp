#include <iostream>
#include <fstream>
#include <vector>
#include <atomic>
#include <zlib.h>

std::atomic<bool> found(false); 

uint32_t calculate_crc32(const std::vector<uint8_t>& data) {
    return crc32(0, data.data(), data.size());
}

void worker(std::vector<uint8_t> base, 
            uint32_t target_crc,
            uint32_t start, uint32_t end,
            std::vector<uint8_t>* result_ptr) {
    // calculate prefix for optimization
    uint32_t prefix_crc = crc32(0, base.data(), base.size());

    // calculate suffix
    std::vector<uint8_t> suffix(4); // 4 байта под подбор

    for (uint32_t value = start; value < end && !found.load(); ++value) {
        suffix[0] = (value >> 24) & 0xFF;
        suffix[1] = (value >> 16) & 0xFF;
        suffix[2] = (value >> 8) & 0xFF;
        suffix[3] = value & 0xFF;

        uint32_t final_crc = crc32(prefix_crc, suffix.data(), 4);

        if (final_crc == target_crc) {
            found.store(true);

            if (result_ptr) {
                base.insert(base.end(), suffix.begin(), suffix.end());
                *result_ptr = std::move(base);
            }

            return;
        }
    }
}
