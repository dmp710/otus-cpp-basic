#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <map>
#include <vector>
#include <chrono>
#include <thread>
#include <mutex>
#include <queue>
#include <iterator>

const size_t TOPK = 10;
using Counter = std::map<std::string, std::size_t>;

std::string normalize(const std::string &str);
void count_words(std::istream &stream, Counter &);
void print_topk(std::ostream &stream, const Counter &, const size_t k);
void merge_sorted_counters(const std::vector<Counter> &local_counters, Counter &final_counter);

struct IteratorState {
    Counter::const_iterator it;
    Counter::const_iterator end;
    size_t index;

    bool operator>(const IteratorState &other) const {
        return it->first > other.it->first;
    }
};

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
    merge_sorted_counters(local_counters, global_counter);

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


void merge_sorted_counters(const std::vector<Counter>& local_counters, Counter& final_counter) {
    // Копируем итераторы на начало каждого Counter
    std::priority_queue<IteratorState, std::vector<IteratorState>, std::greater<>> min_heap;
    for (size_t i = 0; i < local_counters.size(); ++i) {
        if (!local_counters[i].empty()) {
            min_heap.push(IteratorState{local_counters[i].begin(), local_counters[i].end(), i});
        }
    }

    while (!min_heap.empty()) {
        auto current = min_heap.top();
        min_heap.pop();

        const auto &[key, count] = *current.it;

        // Вставляем в конец, предполагая, что порядок ключей строго возрастающий
        auto inserted = final_counter.insert({key, count});
        if (!inserted.second) {
            inserted.first->second += count;
        }

        ++current.it;
        if (current.it != current.end) {
            min_heap.push(current);
        }
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