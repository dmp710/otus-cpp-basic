#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <algorithm>
#include <chrono>
#include <thread>
#include <iomanip>

const size_t TOPK = 10;
using Counter = std::map<std::string, std::size_t>;

std::string normalize(const std::string &str);
void count_words(std::istream &stream, Counter &);
void print_topk(std::ostream &stream, const Counter &, const size_t k);

// Обрабатывает список файлов и заполняет локальный счётчик
void process_files(const std::vector<std::string> &filenames, Counter &local_counter)
{
    for (const auto &filename : filenames)
    {
        std::ifstream stream(filename);
        if (!stream.is_open())
        {
            std::cerr << "Failed to open file " << filename << '\n';
            continue;
        }

        std::string word;
        while (stream >> word)
        {
            auto norm = normalize(word);
            if (!norm.empty())
                ++local_counter[norm];
        }
    }
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        std::cerr << "Usage: topk_words [FILES...]\n";
        return EXIT_FAILURE;
    }

    auto start = std::chrono::high_resolution_clock::now();

    // Определяем количество потоков, минимум — 2
    int num_threads = std::thread::hardware_concurrency();
    if (num_threads == 0)
        num_threads = 2;

    std::vector<std::thread> threads;
    std::vector<std::vector<std::string>> file_groups(num_threads);
    std::vector<Counter> local_counters(num_threads);

    // Распределяем файлы между потоками по кругу
    int group_index = 0;
    for (int i = 1; i < argc; ++i)
    {
        file_groups[group_index].push_back(argv[i]);
        group_index = (group_index + 1) % num_threads;
    }

    // Запускаем потоки
    for (int i = 0; i < num_threads; ++i)
    {
        if (!file_groups[i].empty())
        {
            threads.emplace_back([&, i]()
                                 { process_files(file_groups[i], local_counters[i]); });
        }
    }

    for (auto &t : threads)
    {
        t.join();
    }

    // Объединяем результаты
    Counter final_counter;
    for (Counter &c : local_counters)
    {
        for (auto &[word, count] : c)
        {
            final_counter[word] += count;
        }
    }

    // Печатаем топ-K
    print_topk(std::cout, final_counter, TOPK);

    auto end = std::chrono::high_resolution_clock::now();
    auto elapsed_us = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "Elapsed time is " << elapsed_us.count() << " us\n";

    return EXIT_SUCCESS;
}

// Преобразует строку в нижний регистр
std::string to_lower(const std::string &str)
{
    std::string lower_str;
    std::transform(str.begin(), str.end(), std::back_inserter(lower_str),
                   [](unsigned char c)
                   { return std::tolower(c); });
    return lower_str;
}

// Выводит топ-K слов
void print_topk(std::ostream &stream, const Counter &counter, const size_t k)
{
    std::vector<Counter::const_iterator> words;
    words.reserve(counter.size());

    for (auto it = counter.begin(); it != counter.end(); ++it)
    {
        words.push_back(it);
    }

    std::partial_sort(words.begin(), words.begin() + std::min(k, words.size()), words.end(),
                      [](const auto &lhs, const auto &rhs)
                      { return lhs->second > rhs->second; });

    for (size_t i = 0; i < k && i < words.size(); ++i)
    {
        stream << std::setw(4) << words[i]->second << " " << words[i]->first << '\n';
    }
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