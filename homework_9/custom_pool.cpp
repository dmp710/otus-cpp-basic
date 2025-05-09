#include <queue>
#include <string>
#include <mutex>
#include <thread>
#include <map>
#include <vector>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <chrono>
#include <algorithm>

struct task
{
    std::string filename;
};

const size_t TOPK = 10;
using Counter = std::map<std::string, std::size_t>;

class custom_pool
{
private:
    std::queue<task> q;
    std::vector<std::thread> threads;
    std::mutex m;
    std::vector<Counter> &local_counters;
public:
    custom_pool(int num_threads, std::function<void(const task&, Counter&)> func, std::vector<Counter>& c): local_counters(c)
    {
        for (int i = 0; i < num_threads; i++)
        {
            threads.emplace_back([&, i](){
                task t;
                while (true)
                {
                    std::cout << std::this_thread::get_id() << std::endl;
                    auto is = get(t);
                    if (!is) { 
                        return; 
                    }

                    func(t, local_counters[i]);
                } 
            });
        }
    }
    ~custom_pool(){
        for (auto &t :threads) t.join();
    }

    bool get(task &t)
    {
        std::lock_guard lock(m);
        if (q.empty())
            return false;

        t = q.front();
        q.pop();

        return true;
    }

    void push(task t)
    {   
        q.push(t);
    }
};

std::string normalize(const std::string &str);
void count_words(std::istream &stream, Counter &);
void print_topk(std::ostream &stream, const Counter &, const size_t k);

// Обрабатывает список файлов и заполняет локальный счётчик
void process_files(const task t, Counter &local_counter)
{
    std::ifstream stream(t.filename);
    
    if (!stream.is_open())
    {
        std::cerr << "Failed to open file " << t.filename << '\n';
        return;
    }

    std::string word;
    while (stream >> word)
    {
        auto norm = normalize(word);
        if (!norm.empty())
            ++local_counter[norm];
    }
}

int main(int argc, char *argv[]) {
    int num_threads = 4;
    auto start = std::chrono::high_resolution_clock::now();

    std::vector<Counter> local_counters(num_threads);
    {
        custom_pool cp{num_threads, process_files, local_counters};

        for (int i = 0; i < 16; i++)
        {
            task t{"./getsby.txt"};
            cp.push(t);
        }
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