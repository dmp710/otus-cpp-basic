#include <iostream>
#include <pqxx/pqxx>
#include <unordered_map>

#include "WorkScheduler.h"
#include "utils.h"
#include "Database.h"


#include <ftxui/component/component.hpp>  // Menu, Renderer
#include <ftxui/component/screen_interactive.hpp>  // ScreenInteractive
using namespace ftxui;

int WorkScheduler::draw_enter_menu()
{
    std::vector<std::string> options = {
        "🔐 Login", 
        "📝 Register", 
        "❌ Exit"
    };
    int selected = 0;

    auto menu = Menu(&options, &selected);

    auto renderer = Renderer(menu, [&] {
        return vbox({
            text("Добро пожаловать в WorkScheduler!") 
                | center 
                | bold 
                | color(Color::Green),

            separator(),

            text("Используйте стрелки ↑ ↓ и Enter для выбора") 
                | center 
                | dim,

            menu->Render() 
                | frame 
                | borderRounded 
                | size(WIDTH, GREATER_THAN, 30) // минимальная ширина 40
                | center,

        }) | center | vcenter;
    });

    auto screen = ScreenInteractive::TerminalOutput();

    Component component = CatchEvent(renderer, [&](Event event) {
        if (event == Event::Return) {
            screen.ExitLoopClosure()();
            return true;
        }
        return false;
    });

    screen.Loop(component);

    return selected;
}

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
