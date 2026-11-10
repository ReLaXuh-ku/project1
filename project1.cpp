#include <iostream>
#include <vector>
#include <memory>
#include <cstdlib>
#include <ctime>
#include <string>
#include <limits>
#include "GameField.h"
#include "UnitFactory.h"

void clear_input()
{
    std::cin.clear();
    std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
}

int main()
{
    setlocale(LC_ALL, "Russian");
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    const int FIELD_WIDTH = 10;
    const int FIELD_HEIGHT = 10;
    const size_t MAX_UNITS = 6;

    GameField field(FIELD_WIDTH, FIELD_HEIGHT, MAX_UNITS);

    std::cout << "=================================================\n";
    std::cout << "1:Брузер | 2:Самурай | 3:Солдат | 4:Маг | 5:Хилер | 6:Канонир\n";
    std::cout << "=================================================\n";

    for (int i = 0; i < 3; ++i)
    {
        int choice = 0;
        while (true)
        {
            std::cout << "Выберите класс для Юнита [" << (i + 1) << "]: ";
            if (std::cin >> choice && choice >= 1 && choice <= 6) break;
            std::cout << "Неверно! Введите цифру от 1 до 6.\n";
            clear_input();
        }

        COORD spawn_pos = { 0, static_cast<SHORT>(i * 3 + 1) };
        UnitType t = static_cast<UnitType>(choice - 1);
        field.add_unit(UnitFactory::create_unit(t, spawn_pos));
    }

    for (int i = 0; i < 3; ++i)
    {
        int random_choice = (std::rand() % 6);
        COORD spawn_pos = { static_cast<SHORT>(FIELD_WIDTH - 1), static_cast<SHORT>(i * 3 + 1) };
        field.add_unit(UnitFactory::create_unit(static_cast<UnitType>(random_choice), spawn_pos));
    }

    field.add_log("Игра началась! Армии размещены.");

    while (true)
    {
        field.draw();

        size_t player_alive = 0;
        size_t bot_alive = 0;
        for (size_t i = 0; i < field.get_units_count(); ++i)
        {
            if (field.get_unit_by_index(i).is_alive())
            {
                if (i < 3) player_alive++;
                else bot_alive++;
            }
        }

        if (player_alive == 0)
        {
            std::cout << "\n ВЫ ПРОИГРАЛИ! Команда Бота победила.\n";
            break;
        }
        if (bot_alive == 0)
        {
            std::cout << "\n ПОБЕДА! Вы уничтожили команду бота.\n";
            break;
        }

        std::cout << "\n>>> ВАШ ХОД <<<\n";

        bool move_success = false;
        while (!move_success)
        {
            int move_idx = 0;
            std::cout << "Перемещение - выберите живого юнита (1-3) или 0 для ПРОПУСКА: ";
            if (!(std::cin >> move_idx) || move_idx < 0 || move_idx > 3)
            {
                std::cout << "Ошибка! Введите 0, 1, 2 или 3.\n";
                clear_input();
                continue;
            }

            if (move_idx == 0)
            {
                field.add_log("Игрок: пропустил фазу перемещения");
                move_success = true;
                continue;
            }

            if (!field.get_unit_by_index(move_idx - 1).is_alive())
            {
                std::cout << "Ошибка: этот боец погиб!\n";
                continue;
            }

            std::string dir_str;
            std::cout << "Направление (W-вверх, A-влево, S-вниз, D-вправо): ";
            std::cin >> dir_str;

            if (dir_str.length() != 1)
            {
                std::cout << "Ошибка: введите один символ!\n";
                continue;
            }

            char dir = dir_str[0];
            COORD current_pos = field.get_unit_by_index(move_idx - 1).get_position();
            COORD new_pos = current_pos;

            int unit_speed = field.get_unit_by_index(move_idx - 1).get_speed_value();
            if (dir == 'w' || dir == 'W') new_pos.Y -= unit_speed;
            else if (dir == 's' || dir == 'S') new_pos.Y += unit_speed;
            else if (dir == 'a' || dir == 'A') new_pos.X -= unit_speed;
            else if (dir == 'd' || dir == 'D') new_pos.X += unit_speed;
            else
            {
                std::cout << "Ошибка: неверный символ направления!\n";
                continue;
            }

            std::string error_msg = "";
            if (field.move_unit_by_index(move_idx - 1, new_pos, error_msg))
            {
                field.add_log("Игрок: Юнит " + std::to_string(move_idx) + " сместился");
                move_success = true;
            }
            else
            {
                std::cout << "ОТКАТ ХОДА: " << error_msg << " Попробуйте снова.\n";
            }
        }

        field.draw();

        bool action_success = false;
        while (!action_success)
        {
            int action_idx = 0;
            std::cout << "Действие - выберите живого юнита (1-3) или 0 для ПРОПУСКА: ";
            if (!(std::cin >> action_idx) || action_idx < 0 || action_idx > 3)
            {
                std::cout << "Ошибка! Введите 0, 1, 2 или 3.\n";
                clear_input();
                continue;
            }

            if (action_idx == 0)
            {
                field.add_log("Игрок: пропустил фазу действия");
                action_success = true;
                continue;
            }

            if (!field.get_unit_by_index(action_idx - 1).is_alive())
            {
                std::cout << "Ошибка: этот боец погиб!\n";
                continue;
            }

            int target_idx = 0;
            std::cout << "Выберите живую цель (1-6): ";
            if (!(std::cin >> target_idx) || target_idx < 1 || target_idx > 6 || !field.get_unit_by_index(target_idx - 1).is_alive())
            {
                std::cout << "Ошибка: неверная цель!\n";
                clear_input();
                continue;
            }

            Unit& attacker = field.get_unit_by_index(action_idx - 1);
            Unit& target = field.get_unit_by_index(target_idx - 1);

            if (attacker.get_type() != UnitType::healer && target_idx <= 3)
            {
                std::cout << "ОТКАТ ДЕЙСТВИЯ: Атака союзников запрещена!\n";
                continue;
            }
            if (attacker.get_type() == UnitType::healer && target_idx > 3)
            {
                std::cout << "ОТКАТ ДЕЙСТВИЯ: Целитель не лечит врагов!\n";
                continue;
            }

            if (!attacker.can_attack(target.get_position()))
            {
                std::cout << "ОТКАТ ДЕЙСТВИЯ: Цель вне радиуса действия!\n";
                continue;
            }

            attacker.execute_action(target);
            field.add_log("Игрок: Юнит " + std::to_string(action_idx) + " применил навык на [" + std::to_string(target_idx) + "]");
            action_success = true;
        }

        field.remove_dead_units();
        field.draw();

        std::vector<size_t> active_bots;
        for (size_t i = 3; i < field.get_units_count(); ++i)
        {
            if (field.get_unit_by_index(i).is_alive()) active_bots.push_back(i);
        }

        if (!active_bots.empty())
        {
            std::cout << "\nХодит Бот. Подождите...\n";
            Sleep(1000);

            size_t bot_mover = active_bots[std::rand() % active_bots.size()];
            COORD bot_pos = field.get_unit_by_index(bot_mover).get_position();
            int bot_speed = field.get_unit_by_index(bot_mover).get_speed_value();

            bot_pos.X -= bot_speed;
            if (bot_pos.X < 0) bot_pos.X = 0;

            std::string bot_err = "";
            if (field.move_unit_by_index(bot_mover, bot_pos, bot_err))
            {
                field.add_log("Бот: Сместил юнита [" + std::to_string(bot_mover + 1) + "]");
            }

            bool bot_action_done = false;
            std::vector<size_t> player_targets;
            for (size_t i = 0; i < 3; ++i)
            {
                if (i < field.get_units_count() && field.get_unit_by_index(i).is_alive()) player_targets.push_back(i);
            }

            if (!player_targets.empty())
            {
                for (size_t bot_idx : active_bots)
                {
                    Unit& b_attacker = field.get_unit_by_index(bot_idx);

                    for (size_t p_idx : player_targets)
                    {
                        Unit& p_target = field.get_unit_by_index(p_idx);

                        if (b_attacker.can_attack(p_target.get_position()))
                        {
                            b_attacker.execute_action(p_target);
                            field.add_log("Бот: Юнит [" + std::to_string(bot_idx + 1) + "] атаковал цель " + std::to_string(p_idx + 1));
                            bot_action_done = true;
                            break;
                        }
                    }
                    if (bot_action_done) break;
                }

                if (!bot_action_done)
                {
                    field.add_log("Бот: пропустил фазу действия (нет целей)");
                }
            }
        }

        field.remove_dead_units();
    }

    return 0;
}
