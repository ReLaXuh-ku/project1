#pragma once
#include <vector>
#include <memory>
#include <string>
#include <Windows.h>
#include "Unit.h"

class GameField
{
private:
    int width;
    int height;
    size_t max_capacity;
    std::vector<std::unique_ptr<Unit>> units;
    std::vector<std::string> action_log;

public:
    GameField(int w, int h, size_t max_cap);

    ~GameField();
    GameField(const GameField& other);
    GameField& operator=(const GameField& other);
    GameField(GameField&& other) noexcept;
    GameField& operator=(GameField&& other) noexcept;

    bool add_unit(std::unique_ptr<Unit> unit);
    bool remove_dead_units();

    bool move_unit_by_index(size_t index, COORD new_pos, std::string& error_msg);

    size_t get_units_count() const { return units.size(); }
    Unit& get_unit_by_index(size_t index) { return *units[index]; }
    const std::vector<std::unique_ptr<Unit>>& get_all_units() const { return units; }

    void add_log(const std::string& message);
    void draw() const;

    class GameFieldIterator
    {
    private:
        std::vector<std::unique_ptr<Unit>>::iterator current;
    public:
        GameFieldIterator(std::vector<std::unique_ptr<Unit>>::iterator it) : current(it) {}
        bool operator!=(const GameFieldIterator& other) const { return current != other.current; }
        GameFieldIterator& operator++() { ++current; return *this; }
        Unit& operator*() const { return **current; }
    };

    class ConstGameFieldIterator
    {
    private:
        std::vector<std::unique_ptr<Unit>>::const_iterator current;
    public:
        ConstGameFieldIterator(std::vector<std::unique_ptr<Unit>>::const_iterator it) : current(it) {}
        bool operator!=(const ConstGameFieldIterator& other) const { return current != other.current; }
        ConstGameFieldIterator& operator++() { ++current; return *this; }
        const Unit& operator*() const { return **current; }
    };

    GameFieldIterator begin() { return GameFieldIterator(units.begin()); }
    GameFieldIterator end() { return GameFieldIterator(units.end()); }
    ConstGameFieldIterator begin() const { return ConstGameFieldIterator(units.begin()); }
    ConstGameFieldIterator end() const { return ConstGameFieldIterator(units.end()); }
};
