#include <iostream>
#include <pqxx/pqxx>

#include "Database.h"
#include "Postgres.h"
#include "utils.h"

Postgres::Postgres(const std::string& connString) {
    conn_ = std::make_unique<pqxx::connection>(connString);

    if (!conn_ || !conn_->is_open()) {
        throw std::runtime_error("Failed to open database connection");
    }

    // Подготовка всех используемых запросов
    conn_->prepare("check_user_exists",
                   "SELECT 1 FROM users WHERE email = $1 LIMIT 1");

    conn_->prepare("insert_user",
                   "INSERT INTO users (email, hash) VALUES ($1, $2)");

    conn_->prepare("get_user_credentials",
                   "SELECT user_id, hash FROM users WHERE email = $1 LIMIT 1");

    conn_->prepare("get_tasks_for_day",
                   "SELECT task_name, time_start, time_end, description "
                   "FROM actions "
                   "WHERE user_id = $1 AND time_start::date = $2 "
                   "ORDER BY time_start");

    conn_->prepare("get_immediate_tasks",
                   "SELECT task_name, time_start, time_end, description "
                   "FROM actions "
                   "WHERE user_id = $1 AND time_start >= $2 AND time_start <= $3 "
                   "ORDER BY time_start");

    conn_->prepare("check_task_overlap",
                   "SELECT task_name FROM actions "
                   "WHERE user_id = $1 AND (time_start < $3 AND time_end > $2)");

    conn_->prepare("insert_task",
                   "INSERT INTO actions (user_id, task_name, description, time_start, time_end) "
                   "VALUES ($1, $2, $3, $4, $5)");
}

bool Postgres::createUser(const std::string& email, const std::string& password) {
    if (!conn_ || !conn_->is_open()) {
        throw std::runtime_error("Database connection is not available");
    }

    std::string passwordHash = hashPassword(password);

    try {
        pqxx::work txn(*conn_);

        pqxx::result result = txn.exec_prepared("check_user_exists", email);

        if (!result.empty()) {
            std::cerr << "User already exists: " << email << std::endl;
            return false;
        }

        txn.exec_prepared("insert_user", email, passwordHash);

        txn.commit();
        return true;
    } catch (const pqxx::sql_error& e) {
        std::cerr << "SQL error: " << e.what() << "\nQuery: " << e.query() << std::endl;
        return false;
    } catch (const std::exception& e) {
        std::cerr << "Error creating user: " << e.what() << std::endl;
        return false;
    }
}

int Postgres::login(const std::string& email, const std::string& password) {
    if (!conn_ || !conn_->is_open()) {
        throw std::runtime_error("Database connection is not available");
    }

    try {
        pqxx::work txn(*conn_);

        pqxx::result result = txn.exec_prepared("get_user_credentials", email);

        if (result.empty()) {
            std::cerr << "User not found: " << email << std::endl;
            return 0;
        }

        std::string storedHash = result[0]["hash"].as<std::string>();

        if (storedHash == hashPassword(password)) {
            return result[0]["user_id"].as<int>();
        } else {
            return 0;
        }
    } catch (const std::exception& e) {
        std::cerr << "Login error: " << e.what() << std::endl;
        return 0;
    }
}
