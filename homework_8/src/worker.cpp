#include <iostream>
#include <fstream>
#include <vector>
#include <atomic>
#include <zlib.h>

std::atomic<bool> found(false);

void worker(std::vector<uint8_t> base,
            uint32_t target_crc,
            uint64_t start, uint64_t end,
            std::vector<uint8_t> *result_ptr)
{
    // Вычисляем CRC32 от base (то есть: оригинал + "He-he-he")
    uint32_t prefix_crc = crc32(0, base.data(), base.size());

    std::cout << "target CRC32 " << std::hex << target_crc << std::endl;

    std::vector<uint8_t> suffix(4); // 4 байта под подбор

    for (uint64_t value = start; value < end && !found.load(); ++value)
    {
        suffix[0] = (value >> 24) & 0xFF;
        suffix[1] = (value >> 16) & 0xFF;
        suffix[2] = (value >> 8) & 0xFF;
        suffix[3] = value & 0xFF;

        // Проверяем CRC всего: base + 4 байта
        uint32_t final_crc = crc32(prefix_crc, suffix.data(), 4);

        if (final_crc == target_crc)
        {
            found.store(true);

            if (result_ptr)
            {
                base.insert(base.end(), suffix.begin(), suffix.end());
                *result_ptr = std::move(base);
            }

            return;
        }
    }
}
