#include <iostream>
#include <pqxx/pqxx>
#include <thread>
#include <regex>
#include <chrono>

#include "WorkScheduler.h"
#include "utils.h"
#include "utils_time.h"

void WorkScheduler::draw_action_menu()
{
    std::cout << "\n=== Меню расписания ===" << std::endl;
    std::cout << "1. Посмотреть расписание на сегодня" << std::endl;
    std::cout << "2. Посмотреть расписание на определённую дату" << std::endl;
    std::cout << "3. Добавить задачу" << std::endl;
    std::cout << "4. Выход\n> ";
}

void WorkScheduler::schedule()
{
    remind = true;
    std::thread t([&]()
                  {
        while(remind) {
            reminder_loop();
            std::this_thread::sleep_for(std::chrono::minutes(1));
        } });

    t.detach();

    while (true)
    {
        draw_action_menu();

        int choice;
        std::cin >> choice;

        if (std::cin.fail())
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Ошибка ввода. Попробуйте снова.\n";
            continue;
        }

        if (choice == 1)
        {
            // Получаем сегодняшнюю дату
            std::time_t now = std::time(nullptr);
            std::tm *now_tm = std::localtime(&now);
            char buf[11];
            std::strftime(buf, sizeof(buf), "%Y-%m-%d", now_tm);
            std::string today(buf);

            print_tasks_for_date(today);
        }
        else if (choice == 2)
        {
            std::string date;
            std::cout << "Введите дату (YYYY-MM-DD): ";
            std::cin >> date;

            if (!is_valid_date(date))
            {
                std::cout << "Неверный формат даты. Используйте YYYY-MM-DD.\n";
                continue;
            }

            print_tasks_for_date(date);
        }
        else if (choice == 3)
        {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // очистка ввода

            std::string name, description, time_start, time_end;

            std::cout << "Название задачи: ";
            std::getline(std::cin, name);

            std::cout << "Описание: ";
            std::getline(std::cin, description);

            std::cout << "Время начала (YYYY-MM-DD HH:MM): ";
            std::getline(std::cin, time_start);

            if (!is_valid_datetime(time_start))
            {
                std::cout << "Неверный формат времени начала.\n";
                continue;
            }

            std::cout << "Время окончания (YYYY-MM-DD HH:MM): ";
            std::getline(std::cin, time_end);

            if (!is_valid_datetime(time_end))
            {
                std::cout << "Неверный формат времени окончания.\n";
                continue;
            }

            auto tp_start = parse_datetime(time_start);
            auto tp_end = parse_datetime(time_end);

            if (tp_start >= tp_end)
            {
                std::cout << "Время окончания должно быть позже времени начала.\n";
                continue;
            }

            if (db->add_task(name, description, time_start, time_end, user_id))
            {
                std::cout << "Задача успешно добавлена.\n";
            }
        }
        else if (choice == 4)
        {
            std::cout << "Выход из расписания.\n";
            break;
        }
        else
        {
            std::cout << "Неверный выбор. Попробуйте снова.\n";
        }
    }
}

void WorkScheduler::print_tasks_for_date(const std::string &date)
{
    auto tasks = db->get_tasks(date, user_id);

    if (tasks.empty())
    {
        std::cout << "Нет задач на эту дату.\n";
    }
    else
    {
        std::cout << std::left
                  << std::setw(20) << "Задача"
                  << std::setw(25) << "Начало"
                  << std::setw(25) << "Конец"
                  << "Описание\n";
        std::cout << std::string(90, '-') << "\n";

        for (const auto &task : tasks)
        {
            std::cout << std::left
                      << std::setw(20) << task.name
                      << std::setw(25) << task.time_start
                      << std::setw(25) << task.time_end
                      << task.description << "\n";
        }
    }
}

void WorkScheduler::reminder_loop()
{
    while (true)
    {
        try
        {
            auto tasks = db->get_immediate_tasks(61, user_id);

            for (const auto &task : tasks)
            {
                std::string name = task.name;
                std::string start_str = task.time_start;

                // преобразование с учётом часовых поясов
                auto start_time = parse_datetime(start_str);

                auto now = std::chrono::system_clock::now();
                auto diff = std::chrono::duration_cast<std::chrono::minutes>(start_time - now).count();

                if (diff == 60 || diff == 30 || diff == 5)
                {
                    std::string message = "Задача \"" + name + "\" начнётся через " + std::to_string(diff) + " минут.";
                    std::string command = "notify-send 'Напоминание' '" + message + "'";
                    system(command.c_str());

                    std::cout << "🔔 Уведомление отправлено: " << name << " (" << start_str << ")\n";
                }
            }
        }
        catch (const std::exception &e)
        {
            std::cerr << "Ошибка в напоминалке: " << e.what() << std::endl;
        }

        std::this_thread::sleep_for(std::chrono::minutes(1));
    }
}
