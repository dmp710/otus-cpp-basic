#include <iostream>
#include <pqxx/pqxx>

#include "Database.h"
#include "Postgres.h"
#include "utils.h"

Postgres::Postgres(const std::string& connString) {
    conn_ = std::make_unique<pqxx::connection> (connString);
    
    if (!conn_ -> is_open()) {
        throw std::runtime_error("");
    }
};

bool Postgres::createUser(const std::string& email, const std::string& password) {
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


int Postgres::login(const std::string& email, const std::string& password) {
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
            return result[0]["user_id"].as<int>();
        } else {
            return 0;
        }
    } catch (const std::exception& e) {
        std::cerr << "Login error: " << e.what() << std::endl;
        return 0;
    }
}