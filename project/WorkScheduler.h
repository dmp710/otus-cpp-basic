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

    int draw_enter_menu();
    int draw_action_menu();

    void schedule();

    void print_tasks(const std::string &date);

    void reminder_loop();
};