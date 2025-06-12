#include <iostream>
#include <pqxx/pqxx>
#include <unordered_map>

#include "WorkScheduler.h"
#include "utils.h"
#include "Database.h"
#include "utils_time.h"

#include <ftxui/component/component.hpp>  // Menu, Renderer
#include <ftxui/component/screen_interactive.hpp>  // ScreenInteractive
using namespace ftxui;

void WorkScheduler::start()
{
    std::thread t (&WorkScheduler::reminder_loop, this);
    t.detach();

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
                user_id = 0;
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


void WorkScheduler::reminder_loop()
{
    while (true)
    {
        if (!user_id) {
            std::this_thread::sleep_for(std::chrono::minutes(1));
            continue;
        }

        try
        {   
            auto tasks = db-> get_immediate_tasks(61, user_id);

            for (const auto &task : tasks)
            {
                std::string name = task.name;
                std::string start_str = task.time_start;

                // преобразование с учётом часовых поясов
                auto start_time = parse_datetime(start_str);

                auto now = std::chrono::system_clock::now();
                auto diff = std::chrono::duration_cast<std::chrono::minutes>(start_time - now).count();

                if (diff == 60 || diff == 30 || diff == 5)
                {
                    std::string message = "Задача \"" + name + "\" начнётся через " + std::to_string(diff) + " минут.";
                    std::string command = "notify-send 'Напоминание' '" + message + "'";
                    system(command.c_str());

                    std::cout << "🔔 Уведомление отправлено: " << name << " (" << start_str << ")\n";
                }
            }
        }
        catch (const std::exception &e)
        {
            std::cerr << "Ошибка в напоминалке: " << e.what() << std::endl;
        }
    }
}