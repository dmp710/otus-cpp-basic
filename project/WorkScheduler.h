#pragma once

#include <string>
#include <pqxx/pqxx>

struct Task
{
    std::string name;
    std::string time_start;
    std::string time_end;
    std::string description;
};


class WorkScheduler {
public:
    explicit WorkScheduler(const std::string& connectionString);
    ~WorkScheduler(); 

    void start();

    bool createUser(const std::string& email, const std::string& password);
    bool login(const std::string& email, const std::string& password);
private:
    std::unique_ptr<pqxx::connection> conn_;

    int user_id = -1;
    bool remind;

    void schedule();

    std::vector<Task> get_tasks(const std::string &date_str);

    bool add_task(const std::string &name,
        const std::string &description,
        const std::string &time_start,
        const std::string &time_end);
    
    void print_tasks_for_date(const std::string &date);

    void reminder_loop();
};