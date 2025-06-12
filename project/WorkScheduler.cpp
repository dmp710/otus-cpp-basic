#include <iostream>
#include <pqxx/pqxx>
#include <thread>
#include <chrono>

#include "WorkScheduler.h"
#include "utils_time.h"

void WorkScheduler::schedule() {
    while (true) {
        int choice = draw_action_menu();

        if (choice == 1) {
            std::time_t now = std::time(nullptr);
            std::tm* now_tm = std::localtime(&now);
            char buf[11];
            std::strftime(buf, sizeof(buf), "%Y-%m-%d", now_tm);
            print_tasks(buf);

        } else if (choice == 2) {
            std::string date;
            std::cout << "Введите дату (YYYY-MM-DD): ";
            std::cin >> date;

            if (!is_valid_date(date)) {
                std::cout << "Неверный формат даты. Используйте YYYY-MM-DD.\n";
                continue;
            }

            print_tasks(date);

        } else if (choice == 3) {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            std::string name, description, time_start, time_end;

            std::cout << "Название задачи: ";
            std::getline(std::cin, name);
            std::cout << "Описание: ";
            std::getline(std::cin, description);
            std::cout << "Время начала (YYYY-MM-DD HH:MM): ";
            std::getline(std::cin, time_start);

            if (!is_valid_datetime(time_start)) {
                std::cout << "Неверный формат времени начала.\n";
                continue;
            }

            std::cout << "Время окончания (YYYY-MM-DD HH:MM): ";
            std::getline(std::cin, time_end);

            if (!is_valid_datetime(time_end)) {
                std::cout << "Неверный формат времени окончания.\n";
                continue;
            }

            if (parse_datetime(time_start) >= parse_datetime(time_end)) {
                std::cout << "Время окончания должно быть позже начала.\n";
                continue;
            }

            if (db->add_task(name, description, time_start, time_end, user_id)) {
                std::cout << "Задача успешно добавлена.\n";
            }

        } else if (choice == 4) {
            std::cout << "Выход из расписания.\n";
            break;
        }
    }
}

#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>

using namespace ftxui;

void WorkScheduler::print_tasks(const std::string &date)
{
    auto tasks = db->get_tasks(date, user_id);
    auto screen = ScreenInteractive::TerminalOutput();

    std::vector<Element> table_rows;

    // Заголовок таблицы
    table_rows.push_back(hbox({text("Задача") | bold | size(WIDTH, EQUAL, 25),
                               text("Начало") | bold | size(WIDTH, EQUAL, 20),
                               text("Конец") | bold | size(WIDTH, EQUAL, 20),
                               text("Описание") | bold | size(WIDTH, EQUAL, 30)}));

    // Строки задач
    for (const auto &task : tasks)
    {
        table_rows.push_back(hbox({text(task.name) | size(WIDTH, EQUAL, 25),
                                   text(task.time_start) | size(WIDTH, EQUAL, 20),
                                   text(task.time_end) | size(WIDTH, EQUAL, 20),
                                   text(task.description) | size(WIDTH, EQUAL, 30)}));
    }

    if (tasks.empty())
    {
        table_rows.push_back(text("Нет задач на эту дату.") | dim | center);
    }

    // Кнопка выхода
    auto exit_button = Button(" OK ", screen.ExitLoopClosure());

    // Контейнер и рендер
    auto layout = Container::Vertical({exit_button});

    auto renderer = Renderer(layout, [&]
                             { return vbox({text("Задачи на дату: " + date) | center | bold | color(Color::Green),
                                            separator(),
                                            vbox(table_rows) | border | yframe | flex,
                                            separator(),
                                            hbox({filler(), exit_button->Render(), filler()})}); });

    screen.Loop(renderer);
}
