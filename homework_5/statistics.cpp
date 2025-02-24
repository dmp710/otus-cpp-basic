#include <iostream>
#include <limits>
#include <vector>
#include <cmath>
#include <algorithm>

// Абстрактный класс для статистики
class IStatistics {
public:
    virtual ~IStatistics() {}
    virtual void update(double next) = 0;
    virtual double eval() const = 0;
    virtual const char * name() const = 0;
};

// Класс для вычисления минимального значения
class Min : public IStatistics {
public:
    Min() : m_min{std::numeric_limits<double>::infinity()} {}

    void update(double next) override {
        if (next < m_min) {
            m_min = next;
        }
    }

    double eval() const override {
        return m_min;
    }

    const char * name() const override {
        return "min";
    }

private:
    double m_min;
};

// Класс для вычисления максимального значения
class Max : public IStatistics {
public:
    Max() : m_max{std::numeric_limits<double>::lowest()} {}

    void update(double next) override {
        if (next > m_max) {
            m_max = next;
        }
    }

    double eval() const override {
        return m_max;
    }

    const char * name() const override {
        return "max";
    }

private:
    double m_max;
};

// Класс для вычисления среднего значения
class Mean : public IStatistics {
public:
    Mean() : m_sum{0.0}, m_count{0} {}

    void update(double next) override {
        m_sum += next;
        m_count++;
    }

    double eval() const override {
        return m_count == 0 ? 0.0 : m_sum / m_count;
    }

    const char * name() const override {
        return "mean";
    }

private:
    double m_sum;
    size_t m_count;
};

// Класс для вычисления стандартного отклонения
class Std : public IStatistics {
public:
    Std() : m_sum{0.0}, m_sum_of_squares{0.0}, m_count{0} {}

    void update(double next) override {
        m_sum += next;
        m_sum_of_squares += next * next;
        m_count++;
    }

    double eval() const override {
        if (m_count < 2) {
            return 0.0;
        }
        double mean = m_sum / m_count;
        return std::sqrt((m_sum_of_squares / m_count) - (mean * mean));
    }

    const char * name() const override {
        return "std";
    }

private:
    double m_sum;
    double m_sum_of_squares;
    size_t m_count;
};

// Класс для вычисления 90-го процентиля
class Pct : public IStatistics {
    public:
        Pct(double pct) : pct(pct) {
            // Конкатенируем строку через std::to_string
            name_str = "pct" + std::to_string(static_cast<int>(pct));
        }
    
        void update(double next) override {
            m_data.push_back(next);
        }
    
        double eval() const override {
            if (m_data.empty()) return 0.0;
    
            std::vector<double> sorted_data = m_data;
            std::sort(sorted_data.begin(), sorted_data.end());
    
            // Рассчитываем индекс с учетом округления
            size_t idx = static_cast<size_t>(std::round(pct / 100.0 * (sorted_data.size() - 1)));
            if (idx >= sorted_data.size()) {
                idx = sorted_data.size() - 1; // В случае выхода за пределы
            }
    
            return sorted_data[idx];
        }
    
        const char* name() const override {
            return name_str.c_str();
        }
    
    private:
        double pct;
        std::string name_str;  // Используем std::string для имени
        std::vector<double> m_data;
    };
    
    

int main() {
    const size_t statistics_count = 6;
    IStatistics *statistics[statistics_count];

    // Создание объектов для всех статистик
    statistics[0] = new Min{};
    statistics[1] = new Max{};
    statistics[2] = new Mean{};
    statistics[3] = new Std{};
    statistics[4] = new Pct{90};
    statistics[5] = new Pct{95};

    double val = 0;
    while (std::cin >> val) {
        for (size_t i = 0; i < statistics_count; ++i) {
            statistics[i]->update(val);
        }
    }

    // Проверка на ошибки ввода
    if (!std::cin.eof() && std::cin.fail()) {
        std::cerr << "Invalid input data\n";
        return 1;
    }

    // Вывод результатов
    for (size_t i = 0; i < statistics_count; ++i) {
        std::cout << statistics[i]->name() << " = " << statistics[i]->eval() << std::endl;
    }

    // Очистка памяти
    for (size_t i = 0; i < statistics_count; ++i) {
        delete statistics[i];
    }

    return 0;
}
