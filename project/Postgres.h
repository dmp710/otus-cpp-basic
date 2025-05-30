#pragma once

#include <memory>
#include <string>
#include <pqxx/pqxx>
#include "Database.h"

class Postgres : public Database {
public:
    explicit Postgres(const std::string& connString);

    bool createUser(const std::string& email, const std::string& password) override;
    int login(const std::string& email, const std::string& password) override;

    std::vector<Task> get_tasks(const std::string &date_str, const int &user_id) override;
    std::vector<Task> get_immediate_tasks(int t, const int &user_id) override;

    bool add_task(const std::string &name,
        const std::string &description,
        const std::string &time_start,
        const std::string &time_end,
        const int &user_id) override;
    
private:
    std::unique_ptr<pqxx::connection> conn_;
};
