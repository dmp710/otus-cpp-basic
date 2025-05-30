#include <vector>
#include <iostream>

#include "Database.h"
#include "Postgres.h"
#include "utils.h"

std::vector<Task> Postgres::get_tasks(const std::string &date_str, const int &user_id)
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
                "ORDER BY time_start"},
            pqxx::params{user_id, date_str});

        for (const auto &row : result)
        {
            tasks.push_back(Task{
                row["task_name"].c_str(),
                row["time_start"].c_str(),
                row["time_end"].c_str(),
                row["description"].is_null() ? "" : row["description"].c_str()});
        }

        txn.commit();
    }
    catch (const std::exception &e)
    {
        std::cerr << "Ошибка при получении расписания на дату " << date_str << ": " << e.what() << std::endl;
    }

    return tasks;
}


std::vector<Task> Postgres::get_immediate_tasks(int t, const int &user_id)
{
    std::vector<Task> tasks;

    auto now = std::chrono::system_clock::now();
    auto in_min = now + std::chrono::minutes(t); // запас на 1 час вперёд

    std::time_t now_time = std::chrono::system_clock::to_time_t(now);
    std::time_t in_61_time = std::chrono::system_clock::to_time_t(in_min);

    char now_buf[20], in_buf[20];
    std::strftime(now_buf, sizeof(now_buf), "%F %T", std::localtime(&now_time));
    std::strftime(in_buf, sizeof(in_buf), "%F %T", std::localtime(&in_61_time));

    try
    {
        pqxx::work txn(*conn_);

        pqxx::result result = txn.exec(
            pqxx::zview{
                "SELECT task_name, time_start FROM actions "
                "WHERE user_id = $1 AND time_start >= $2 AND time_start <= $3 "
                "ORDER BY time_start"},
            pqxx::params{user_id, now_buf, in_buf});

        for (const auto &row : result)
        {
            tasks.push_back(Task{
                row["task_name"].c_str(),
                row["time_start"].c_str(),
                row["time_end"].c_str(),
                row["description"].is_null() ? "" : row["description"].c_str()});
        }

        txn.commit();
    }
    catch (const std::exception &e)
    {
        std::cerr << "Ошибка при получении ближайших задач " ": " << e.what() << std::endl;
    }

    return tasks;
}

bool Postgres::add_task(const std::string &name,
                             const std::string &description,
                             const std::string &time_start,
                             const std::string &time_end,
                             const int &user_id)
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