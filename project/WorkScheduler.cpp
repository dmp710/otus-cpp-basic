#include <iostream>
#include <pqxx/pqxx>

#include "WorkScheduler.h"
#include "utils.h"

void WorkScheduler::schedule()
{
    while (true)
    {
        std::cout << "\n=== Меню расписания ===" << std::endl;
        std::cout << "1. Посмотреть расписание на сегодня" << std::endl;
        std::cout << "2. Посмотреть расписание на определённую дату" << std::endl;
        std::cout << "3. Добавить задачу" << std::endl;
        std::cout << "4. Выход\n> ";

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

            if (add_task(name, description, time_start, time_end))
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


std::vector<Task> WorkScheduler::get_tasks(const std::string &date_str)
{
    std::vector<Task> tasks;

    try
    {
        pqxx::work txn(*conn_);

        pqxx::result result = txn.exec(
            pqxx::zview{
                "SELECT task_name, time_start, time_end, description "
                "FROM actions "
                "WHERE user_id = $1 AND time_start::date = $2 "
                "ORDER BY time_start"
            },
            pqxx::params{user_id, date_str}
        );

        for (const auto &row : result)
        {
            tasks.push_back(Task{
                row["task_name"].c_str(),
                row["time_start"].c_str(),
                row["time_end"].c_str(),
                row["description"].is_null() ? "" : row["description"].c_str()
            });
        }

        txn.commit();
    }
    catch (const std::exception &e)
    {
        std::cerr << "Ошибка при получении расписания на дату " << date_str << ": " << e.what() << std::endl;
    }

    return tasks;
}


bool WorkScheduler::add_task(const std::string &name,
                             const std::string &description,
                             const std::string &time_start,
                             const std::string &time_end)
{
    try
    {
        pqxx::work txn(*conn_);

        // Проверка на пересечение задач
        pqxx::result overlap_check = txn.exec(
            pqxx::zview{
                "SELECT task_name FROM actions "
                "WHERE user_id = $1 "
                "AND (time_start < $3 AND time_end > $2)"},
            pqxx::params{user_id, time_start, time_end});

        if (!overlap_check.empty())
        {
            std::cout << overlap_check.empty() << std::endl;
            std::string existing_task = overlap_check[0]["task_name"].c_str();
            std::cout << "Ошибка: задача пересекается с существующей — \""
                      << existing_task << "\"\n";
            return false;
        }

        txn.exec(
            pqxx::zview{
                "INSERT INTO actions (user_id, task_name, description, time_start, time_end) "
                "VALUES ($1, $2, $3, $4, $5)"},
            pqxx::params{user_id, name, description, time_start, time_end});

        txn.commit();

        return true;
    }
    catch (const std::exception &e)
    {
        std::cerr << "Ошибка при добавлении задачи: " << e.what() << std::endl;
        return false;
    }
}

void WorkScheduler::print_tasks_for_date(const std::string &date)
{
    std::vector<Task> tasks = get_tasks(date);

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

