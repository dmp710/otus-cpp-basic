#include <iostream>

#include "WorkScheduler.h"
#include "utils.h"

WorkScheduler::WorkScheduler(const std::string &connectionString)
{
    conn_ = std::make_unique<pqxx::connection>(connectionString);
    if (!conn_->is_open())
    {
        throw std::runtime_error("Failed to connect to PostgreSQL database");
    }
}

WorkScheduler::~WorkScheduler() {}

bool WorkScheduler::createUser(const std::string& email, const std::string& password) {
    if (!conn_ || !conn_->is_open()) {
        throw std::runtime_error("Database connection is not available");
    }

    std::string passwordHash = hashPassword(password);

    try {
        pqxx::work txn(*conn_);

        pqxx::result result = txn.exec(
            pqxx::zview{"SELECT 1 FROM users WHERE email = $1 LIMIT 1"},
            pqxx::params{email}
        );

        if (!result.empty()) {
            std::cerr << "User already exists: " << email << std::endl;
            return false;
        }

        txn.exec(
            pqxx::zview{"INSERT INTO users (email, hash) VALUES ($1, $2)"},
            pqxx::params{email, passwordHash}
        );

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


bool WorkScheduler::login(const std::string& email, const std::string& password) {
    if (!conn_ || !conn_->is_open()) {
        throw std::runtime_error("Database connection is not available");
    }

    try {
        pqxx::work txn(*conn_);

        pqxx::result result = txn.exec(
            pqxx::zview{"SELECT user_id, hash FROM users WHERE email = $1 LIMIT 1"},
            pqxx::params{email}
        );

        if (result.empty()) {
            std::cerr << "User not found: " << email << std::endl;
            return false;
        }

        std::string storedHash = result[0]["hash"].as<std::string>();

        if (storedHash == hashPassword(password)) {
            user_id = result[0]["user_id"].as<int>();
            return true;
        } else {
            return false;
        }
    } catch (const std::exception& e) {
        std::cerr << "Login error: " << e.what() << std::endl;
        return false;
    }
}
