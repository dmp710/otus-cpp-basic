#pragma once

#include <string> 

struct Task
{
    std::string name;
    std::string time_start;
    std::string time_end;
    std::string description;
};

class Database {
public:
    virtual ~Database() = default;

    virtual bool createUser(const std::string& email, const std::string& password) = 0;
    virtual int login(const std::string& email, const std::string& password) = 0;

    virtual std::vector<Task> get_tasks(const std::string &date_str, const int &user_id) = 0;
    virtual std::vector<Task> get_immediate_tasks(int t, const int &user_id) = 0;

    virtual bool add_task(const std::string &name,
        const std::string &description,
        const std::string &time_start,
        const std::string &time_end,
        const int &user_id) = 0;
};