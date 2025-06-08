#include <iostream>

#include "WorkScheduler.h"

void WorkScheduler::draw_action_menu()
{
    std::cout << "\n=== Меню расписания ===" << std::endl;
    std::cout << "1. Посмотреть расписание на сегодня" << std::endl;
    std::cout << "2. Посмотреть расписание на определённую дату" << std::endl;
    std::cout << "3. Добавить задачу" << std::endl;
    std::cout << "4. Выход\n> ";
}

#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>

using namespace ftxui;

void WorkScheduler::print_tasks(const std::string& date) {
    auto tasks = db->get_tasks(date, user_id);
    auto screen = ScreenInteractive::TerminalOutput();

    std::vector<Element> table_rows;

    // Заголовок таблицы
    table_rows.push_back(hbox({
        text("Задача")     | bold | size(WIDTH, EQUAL, 25),
        text("Начало")     | bold | size(WIDTH, EQUAL, 20),
        text("Конец")      | bold | size(WIDTH, EQUAL, 20),
        text("Описание")   | bold | size(WIDTH, EQUAL, 30)
    }));

    // Строки задач
    for (const auto& task : tasks) {
        table_rows.push_back(hbox({
            text(task.name)         | size(WIDTH, EQUAL, 25),
            text(task.time_start)   | size(WIDTH, EQUAL, 20),
            text(task.time_end)     | size(WIDTH, EQUAL, 20),
            text(task.description)  | size(WIDTH, EQUAL, 30)
        }));
    }

    if (tasks.empty()) {
        table_rows.push_back(text("Нет задач на эту дату.") | dim | center);
    }

    // Кнопка выхода
    auto exit_button = Button(" OK ", screen.ExitLoopClosure());

    // Контейнер и рендер
    auto layout = Container::Vertical({ exit_button });

    auto renderer = Renderer(layout, [&] {
        return vbox({
            text("Задачи на дату: " + date) | center | bold | color(Color::Green),
            separator(),
            vbox(table_rows) | border | yframe | flex,
            separator(),
            hbox({ filler(), exit_button->Render(), filler() })
        });
    });

    screen.Loop(renderer);
}

