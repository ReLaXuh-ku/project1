#pragma once
#include "Unit.h"

class Healer : public Unit
{
public:
    Healer(COORD pos) : Unit(pos, 250, 5, 50, 999, 1, UnitType::healer) {}

    std::unique_ptr<Unit> clone() const override
    {
        return std::make_unique<Healer>(*this);
    }

    void execute_action(Unit& target) override
    {
        if (can_attack(target.get_position()))
        {
            target.take_healing(attack.get_damage());
        }
    }
};

class Cannoneer : public Unit
{
public:
    Cannoneer(COORD pos) : Unit(pos, 250, 0, 30, 999, 1, UnitType::cannoneer) {}

    std::unique_ptr<Unit> clone() const override
    {
        return std::make_unique<Cannoneer>(*this);
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
