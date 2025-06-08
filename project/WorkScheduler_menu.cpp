#include <iostream>
#include <pqxx/pqxx>
#include <unordered_map>

#include "WorkScheduler.h"
#include "utils.h"
#include "Database.h"


#include <ftxui/component/component.hpp>  // Menu, Renderer
#include <ftxui/component/screen_interactive.hpp>  // ScreenInteractive
using namespace ftxui;

void WorkScheduler::start()
{
    while (true)
    {
        std::string email;
        std::string password;

        int choice = draw_enter_menu();

        if (choice == 2)  // Exit
        {
            std::cout << "Bye" << std::endl;
            return;
        }

        if (choice != 0 && choice != 1)
        {
            std::cout << "Input error." << std::endl;
            continue;
        }

        std::cout << "Enter your email" << std::endl;
        std::cin >> email;

        if (!isValidEmail(email))
        {
            std::cout << "Invalid email, try again" << std::endl;
            continue;
        }

        std::cout << "Enter password" << std::endl;
        std::cin >> password;

        if (choice == 0)  // Login
        {
            user_id = db->login(email, password);

            if (user_id)
            {
                std::cout << "Welcome!" << std::endl;
                schedule();
                remind = false;
            }

            continue;
        }

        if (choice == 1)  // Register
        {
            if (db->createUser(email, password))
            {
                std::cout << "Success register!" << std::endl;
            }
            continue;
        }
    }
}
