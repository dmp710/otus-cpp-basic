#include <iostream>
#include <pqxx/pqxx>
#include <fstream>
#include <unordered_map>

#include "WorkScheduler.h"

int isValidEmail(std::string &email)
{
    auto atPos = email.find("@");
    auto dotPos = email.find(".");

    return atPos != std::string::npos && dotPos != std::string::npos && atPos > 0 && dotPos > atPos + 1 && dotPos < email.size() - 1;
}

void WorkScheduler::start()
{
    while (true)
    {
        int choose;
        std::string email;
        std::string password;

        std::cout << "Choose action" << std::endl;
        std::cout << "1. Login" << std::endl;
        std::cout << "2. Register" << std::endl;
        std::cout << "3. Exit" << std::endl << std::endl;

        std::cin >> choose;

        // Проверка на ошибку ввода
        if (std::cin.fail())
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Очистка буфера
            std::cout << "Некорректный выбор. Ожидалось число." << std::endl;
            return;
        }

        if (choose == 3)
        {
            std::cout << "Bye" << std::endl;
            return;
        }

        if (choose != 1 && choose != 2)
        {
            std::cout << "Ошибка ввода." << std::endl;
            continue;
        }

        std::cout << "Enter your email" << std::endl;
        std::cin >> email;

        if (!isValidEmail(email))
        {
            std::cout << "invalid email, try again" << std::endl;
            continue;
        }

        std::cout << "Enter_password" << std::endl;
        std::cin >> password;

        if (choose == 1)
        {
            if (login(email, password))
            {
                std::cout << "Welcome!" << std::endl;
                schedule();
            }
            continue;
        }

        if (choose == 2)
        {
            if (createUser(email, password))
            {
                std::cout << "Success register!" << std::endl;
            }
            continue;
        }
    }
}

