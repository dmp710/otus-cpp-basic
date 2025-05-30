#include <iostream>
#include <pqxx/pqxx>
#include <unordered_map>

#include "WorkScheduler.h"
#include "utils.h"
#include "Database.h"
#include "Postgres.h"

void run();

int main()
{
    try
    {
        run();
    }
    catch (const pqxx::broken_connection &e)
    {
        std::cerr << "Connection error: " << e.what() << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << "Exception cauth: " << e.what() << std::endl;
    }
    catch (...)
    {
        std::cerr << "Unknown exception" << std::endl;
    }
}

void run()
{
    auto env = readEnvFile(".env");

    std::string connStr =
        "host=" + env["POSTGRES_HOST"] + " "
                                         "port=" +
        env["POSTGRES_PORT"] + " "
                               "user=" +
        env["POSTGRES_USER"] + " "
                               "password=" +
        env["POSTGRES_PASSWORD"] + " "
                                   "dbname=" +
        env["POSTGRES_DB"];

    auto db = std::make_unique<Postgres> (connStr);
    WorkScheduler scheduler(std::move(db));

    scheduler.start();
}