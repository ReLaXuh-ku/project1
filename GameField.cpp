#include "GameField.h"
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <cmath>

std::string get_type_name(UnitType type);

GameField::GameField(int w, int h, size_t max_cap) : width(w < 3 ? 3 : w), height(h < 3 ? 3 : h), max_capacity(max_cap) {}

GameField::~GameField()
{
    units.clear();
}

GameField::GameField(const GameField& other) : width(other.width), height(other.height), max_capacity(other.max_capacity), action_log(other.action_log)
{
    for (const auto& unit : other.units)
    {
        if (unit != nullptr)
        {
            units.push_back(unit->clone());
        }
    }
}

GameField& GameField::operator=(const GameField& other)
{
    if (this == &other)
    {
        return *this;
    }

    units.clear();
    width = other.width;
    height = other.height;
    max_capacity = other.max_capacity;
    action_log = other.action_log;

    for (const auto& unit : other.units)
    {
        if (unit != nullptr)
        {
            units.push_back(unit->clone());
        }
    }
    return *this;
}

GameField::GameField(GameField&& other) noexcept
    : width(other.width), height(other.height), max_capacity(other.max_capacity), units(std::move(other.units)), action_log(std::move(other.action_log))
{
    other.width = 0;
    other.height = 0;
    other.max_capacity = 0;
}

GameField& GameField::operator=(GameField&& other) noexcept
{
    if (this == &other)
    {
        return *this;
    }

    units = std::move(other.units);
    action_log = std::move(other.action_log);
    width = other.width;
    height = other.height;
    max_capacity = other.max_capacity;

    other.width = 0;
    other.height = 0;
    other.max_capacity = 0;

    return *this;
}

bool GameField::add_unit(std::unique_ptr<Unit> unit)
{
    if (units.size() >= max_capacity || unit == nullptr)
    {
        return false;
    }
    units.push_back(std::move(unit));
    return true;
}

bool GameField::remove_dead_units()
{
    bool removedAny = false;
    for (auto it = units.begin(); it != units.end(); )
    {
        if (!(*it)->is_alive())
        {
            add_log("Юнит " + get_type_name((*it)->get_type()) + " погиб!");
            it = units.erase(it);
            removedAny = true;
        }
        else
        {
            ++it;
        }
    }
    return removedAny;
}


bool GameField::move_unit_by_index(size_t index, COORD new_pos, std::string& error_msg)
{
    if (index >= units.size())
    {
        error_msg = "Неверный индекс юнита!";
        return false;
    }

    if (new_pos.X < 0 || new_pos.X >= width || new_pos.Y < 0 || new_pos.Y >= height)
    {
        error_msg = "Попытка выхода за границы поля!";
        return false;
    }

    for (size_t i = 0; i < units.size(); ++i)
    {
        if (i != index && units[i]->get_position().X == new_pos.X && units[i]->get_position().Y == new_pos.Y)
        {
            error_msg = "Эта клетка уже занята другим бойцом!";
            return false;
        }
    }

    if (!units[index]->can_move_to(new_pos))
    {
        error_msg = "Недостаточно скорости или попытка пойти по диагонали!";
        return false;
    }

    units[index]->move_to(new_pos);
    return true;
}

void GameField::add_log(const std::string& message)
{
    action_log.push_back(message);
    if (action_log.size() > 15)
    {
        action_log.erase(action_log.begin());
    }
}

std::string get_type_name(UnitType type)
{
    switch (type)
    {
    case UnitType::bruiser:   return "Брузер (Танк)";
    case UnitType::samurai:   return "Самурай (Танк)";
    case UnitType::soldier:   return "Солдат (Дальник)";
    case UnitType::mage:      return "Маг (Дальник)";
    case UnitType::healer:    return "Целитель (Глобал)";
    case UnitType::cannoneer: return "Канонир (Глобал)";
    default:                  return "Неизвестный";
    }
}

void GameField::draw() const
{
    system("cls");

    std::vector<std::string> panel_lines;

    size_t display_index = 0;
    for (auto it = begin(); it != end(); ++it)
    {
        const Unit& u = *it;
        std::string team = (display_index < 3) ? "Игрок" : "Бот";

        panel_lines.push_back("--- Юнит [" + std::to_string(display_index + 1) + "] ---");
        panel_lines.push_back("Команда: " + team + " | Класс: " + get_type_name(u.get_type()));

        panel_lines.push_back("ХП: " + std::to_string(u.get_health_value()) +
            " | Броня: " + std::to_string(u.get_armor_value()) +
            " | Урон: " + std::to_string(u.get_attack_value()));
        panel_lines.push_back("Радиус: " + std::to_string(u.get_range_value()) +
            " | Скорость: " + std::to_string(u.get_speed_value()));
        panel_lines.push_back("Позиция: (" + std::to_string(u.get_position().X) + ", " + std::to_string(u.get_position().Y) + ")");

        std::string targets = "В радиусе действия: ";
        size_t target_index = 0;
        bool has_targets = false;

        for (auto inner_it = begin(); inner_it != end(); ++inner_it)
        {
            if (display_index != target_index && u.can_attack((*inner_it).get_position()))
            {
                targets += std::to_string(target_index + 1) + " ";
                has_targets = true;
            }
            target_index++;
        }
        if (!has_targets) targets += "нет целей";
        panel_lines.push_back(targets);
        panel_lines.push_back("");

        display_index++;
    }

    size_t current_panel_line = 0;

    for (int x = 0; x < width + 2; ++x) std::cout << "X ";
    std::cout << "    ПАРАМЕТРЫ БОЙЦОВ:" << std::setw(30) << "" << "ЖУРНАЛ ДЕЙСТВИЙ:\n";

    for (int y = 0; y < height; ++y)
    {
        std::cout << "X ";
        for (int x = 0; x < width; ++x)
        {
            int found_unit_index = -1;
            for (size_t i = 0; i < units.size(); ++i)
            {
                if (units[i]->get_position().X == x && units[i]->get_position().Y == y)
                {
                    found_unit_index = static_cast<int>(i);
                    break;
                }
            }

            if (found_unit_index != -1)
            {
                std::cout << (found_unit_index + 1) << " ";
            }
            else
            {
                std::cout << "* ";
            }
        }
        std::cout << "X";

        std::string panel_part = "";
        if (current_panel_line < panel_lines.size())
        {
            panel_part = panel_lines[current_panel_line];
            current_panel_line++;
        }
        std::cout << "    " << std::setw(55) << std::left << panel_part;

        if (y < static_cast<int>(action_log.size()))
        {
            std::cout << " | " << action_log[y];
        }
        std::cout << "\n";
    }

    for (int x = 0; x < width + 2; ++x) std::cout << "X ";
    std::cout << "\n";

    size_t extra_line = height;
    while (current_panel_line < panel_lines.size() || extra_line < action_log.size())
    {
        for (int x = 0; x < width + 2; ++x) std::cout << "  ";

        std::string panel_part = "";
        if (current_panel_line < panel_lines.size())
        {
            panel_part = panel_lines[current_panel_line];
            current_panel_line++;
        }
        std::cout << "    " << std::setw(55) << std::left << panel_part;

        if (extra_line < action_log.size())
        {
            std::cout << " | " << action_log[extra_line];
        }
        std::cout << "\n";
        extra_line++;
    }
}
