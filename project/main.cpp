#include <iostream>
#include <pqxx/pqxx>
#include <unordered_map>

#include "WorkScheduler.h"
#include "utils.h"

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

    WorkScheduler scheduler(connStr);

    scheduler.start();
}