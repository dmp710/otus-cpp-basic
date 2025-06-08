#include <iostream>

#include "WorkScheduler.h"

#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
using namespace ftxui;

int WorkScheduler::draw_action_menu()
{
    std::vector<std::string> options = {
        "📅 Посмотреть расписание на сегодня",
        "📆 Посмотреть расписание на дату",
        "➕ Добавить задачу",
        "❌ Выход"};

    int selected = 0;
    int result = -1;

    auto screen = ScreenInteractive::TerminalOutput();

    MenuOption option;
    option.on_enter = [&]
    {
        result = selected;
        screen.ExitLoopClosure()(); // <-- выход из меню по Enter
    };

    auto menu = Menu(&options, &selected, option);

    auto renderer = Renderer(menu, [&]
                             { return vbox({
                                          text("Меню расписания") | bold | center,
                                          separator(),
                                          menu->Render() | frame | border,
                                      }) |
                                      center | vcenter; });

    screen.Loop(renderer);

    return result + 1; // 
}

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