#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <algorithm>
#include <chrono>
#include <future>
#include <thread>
#include <iomanip>  // Для std::setw
#include "BS_thread_pool.hpp"  // Подключаем BS::thread_pool

const size_t TOPK = 10;
using Counter = std::map<std::string, std::size_t>;

std::string tolower(const std::string &str) {
    std::string lower_str;
    std::transform(str.begin(), str.end(), std::back_inserter(lower_str), [](unsigned char c) {
        return std::tolower(c);
    });
    
    return lower_str;
}

Counter count_words_from_file(const std::string &filename) {
    Counter local_counter;
    std::ifstream input(filename);
    if (!input.is_open()) {
        std::cerr << "Failed to open file " << filename << '\n';
        return {};
    }

    std::string word;
    while (input >> word) {
        ++local_counter[tolower(word)];
    }

    return local_counter;
}

void print_topk(std::ostream &stream, const Counter &counter, const size_t k) {
    std::vector<Counter::const_iterator> words;
    words.reserve(counter.size());

    for (auto it = counter.begin(); it != counter.end(); ++it) {
        words.push_back(it);
    }

    std::partial_sort(words.begin(), words.begin() + k, words.end(),
                      [](const auto &lhs, const auto &rhs) { return lhs->second > rhs->second; });

    for (size_t i = 0; i < k && i < words.size(); ++i) {
        stream << std::setw(4) << words[i]->second << " " << words[i]->first << '\n';
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: topk_words [FILES...]\n";
        return EXIT_FAILURE;
    }

    auto start = std::chrono::high_resolution_clock::now();

    // Создаем пул с количеством потоков, равным количеству ядер
    const int num_threads = std::thread::hardware_concurrency();
    BS::thread_pool pool(num_threads);  // Используем BS::thread_pool

    std::vector<std::future<Counter>> futures;

    for (int i = 1; i < argc; ++i) {
        std::string filename = argv[i];
        futures.push_back(pool.submit_task([filename]() -> Counter {
            return count_words_from_file(filename);
        }));
    }

    // Собираем все результаты
    Counter final_counter;
    for (auto &fut : futures) {
        Counter local = fut.get();
        for (const auto &[word, count] : local) {
            final_counter[word] += count;
        }
    }

    // Выводим топ-10 слов
    print_topk(std::cout, final_counter, TOPK);

    auto end = std::chrono::high_resolution_clock::now();
    auto elapsed_us = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "Elapsed time is " << elapsed_us.count() << " us\n";

    return EXIT_SUCCESS;
}
