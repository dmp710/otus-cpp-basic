#include <iostream>
#include <pqxx/pqxx>
#include <unordered_map>

#include "WorkScheduler.h"
#include "utils.h"

void WorkScheduler::start()
{
    while (true)
    {
        int choice;
        std::string email;
        std::string password;

        std::cout << "Choose action" << std::endl;
        std::cout << "1. Login" << std::endl;
        std::cout << "2. Register" << std::endl;
        std::cout << "3. Exit" << std::endl << std::endl;

        std::cin >> choice;

        // Проверка на ошибку ввода
        if (std::cin.fail())
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Очистка буфера
            std::cout << "Incorrect choice. A number was expected." << std::endl;
            return;
        }

        if (choice == 3)
        {
            std::cout << "Bye" << std::endl;
            return;
        }

        if (choice != 1 && choice != 2)
        {
            std::cout << "Input error." << std::endl;
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

        if (choice == 1)
        {
            if (login(email, password))
            {
                std::cout << "Welcome!" << std::endl;
                schedule();
                remind = false;
            }
            continue;
        }

        if (choice == 2)
        {
            if (createUser(email, password))
            {
                std::cout << "Success register!" << std::endl;
            }
            continue;
        }
    }
}

