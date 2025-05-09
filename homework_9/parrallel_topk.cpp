#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <unordered_map>
#include <vector>
#include <chrono>
#include <thread>
#include <mutex>

const size_t TOPK = 10;
using Counter = std::unordered_map<std::string, std::size_t>;

std::string normalize(const std::string &str);
void count_words(std::istream &stream, Counter &);
void print_topk(std::ostream &stream, const Counter &, const size_t k);
void merge_counters(Counter &target, const Counter &source);

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        std::cerr << "Usage: topk_words [FILES...]\n";
        return EXIT_FAILURE;
    }

    auto start = std::chrono::high_resolution_clock::now();

    int file_count = argc - 1;
    std::vector<std::thread> threads;
    std::vector<Counter> local_counters(file_count);

    for (int i = 0; i < file_count; ++i)
    {
        std::string filename = argv[i + 1];
        threads.emplace_back([filename, &local_counters, i]()
        {
            std::ifstream input{filename};
            if (!input.is_open())
            {
                std::cerr << "Warning: Failed to open file " << filename << '\n';
                return;
            }
            count_words(input, local_counters[i]);
        });
    }

    for (auto &t : threads)
        t.join();

    Counter global_counter;
    for (const auto &local : local_counters)
        merge_counters(global_counter, local);

    print_topk(std::cout, global_counter, TOPK);

    auto end = std::chrono::high_resolution_clock::now();
    auto elapsed_us = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

    std::cout << "Elapsed time is " << elapsed_us.count() << " us\n";
    return EXIT_SUCCESS;
}

std::string normalize(const std::string &str)
{
    std::string clean;
    for (unsigned char ch : str)
    {
        if (std::isalnum(ch))
            clean += std::tolower(ch);
    }
    return clean;
}

void count_words(std::istream &stream, Counter &counter)
{
    std::string word;
    while (stream >> word)
    {
        std::string norm = normalize(word);
        if (!norm.empty())
            ++counter[norm];
    }
}

void merge_counters(Counter &target, const Counter &source)
{
    for (const auto &[word, count] : source)
    {
        target[word] += count;
    }
}

void print_topk(std::ostream &stream, const Counter &counter, const size_t k)
{
    std::vector<Counter::const_iterator> words;
    words.reserve(counter.size());

    for (auto it = std::cbegin(counter); it != std::cend(counter); ++it)
        words.push_back(it);

    size_t actual_k = std::min(k, words.size());

    std::partial_sort(
        words.begin(), words.begin() + actual_k, words.end(),
        [](auto lhs, auto rhs)
        { return lhs->second > rhs->second; });

    std::for_each(
        words.begin(), words.begin() + actual_k,
        [&stream](const Counter::const_iterator &pair)
        {
            stream << std::setw(4) << pair->second << " " << pair->first << '\n';
        });
}
