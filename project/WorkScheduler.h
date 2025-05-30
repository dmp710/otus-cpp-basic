#pragma once

#include <string>
#include <pqxx/pqxx>

#include "Database.h"

class WorkScheduler
{
public:
    explicit WorkScheduler(std::unique_ptr<Database> db)
        : db(std::move(db)) {};

    void start();

private:
    std::unique_ptr<Database> db;

    int user_id = 0;
    bool remind;

    void schedule();

    void print_tasks_for_date(const std::string &date);

    void reminder_loop();
};