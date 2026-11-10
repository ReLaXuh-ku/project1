#pragma once
#include "Unit.h"

class Soldier : public Unit
{
public:
    Soldier(COORD pos) : Unit(pos, 480, 15, 45, 3, 2, UnitType::soldier) {}

    std::unique_ptr<Unit> clone() const override
    {
        return std::make_unique<Soldier>(*this);
    }

    void execute_action(Unit& target) override
    {
        if (can_attack(target.get_position()))
        {
            int final_damage = attack.calculate_damage(target.get_armor());
            target.take_damage(final_damage);
        }
    }
};

class Mage : public Unit
{
public:
    Mage(COORD pos) : Unit(pos, 400, 10, 55, 4, 1, UnitType::mage) {}

    std::unique_ptr<Unit> clone() const override
    {
        return std::make_unique<Mage>(*this);
    }

    void execute_action(Unit& target) override
    {
        if (can_attack(target.get_position()))
        {
            int final_damage = attack.calculate_damage(target.get_armor());
            target.take_damage(final_damage);
        }
    }
};
