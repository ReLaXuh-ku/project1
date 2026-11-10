#pragma once
#include "Unit.h"

class Bruiser : public Unit
{
public:
    Bruiser(COORD pos) : Unit(pos, 800, 30, 50, 1, 1, UnitType::bruiser) {}

    std::unique_ptr<Unit> clone() const override
    {
        return std::make_unique<Bruiser>(*this);
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

class Samurai : public Unit
{
public:
    Samurai(COORD pos) : Unit(pos, 650, 20, 60, 1, 2, UnitType::samurai) {}

    std::unique_ptr<Unit> clone() const override
    {
        return std::make_unique<Samurai>(*this);
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
