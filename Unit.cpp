#include "Unit.h"
#include <algorithm>
#include <cmath>

Unit::Unit(COORD pos, int hp, int arm, int atk, int rng, int s, UnitType t) : position(pos), health(hp), armor(arm), attack(atk), range(rng), speed(s), type(t) {}

COORD Unit::get_position() const
{
    return position;
}

bool Unit::is_alive() const
{
    return !health.is_zero();
}

UnitType Unit::get_type() const
{
    return type;
}

bool Unit::can_attack(COORD target_pos) const
{
    int distance = (std::max)(std::abs(position.X - target_pos.X), std::abs(position.Y - target_pos.Y));
    return distance <= range.get_range();
}

void Unit::take_damage(int final_dmg)
{
    health.decrease(final_dmg);
}
void Unit::take_healing(int amount)
{
    health.increase(amount);
}
bool Unit::can_move_to(COORD target_pos) const
{
    int dist_x = std::abs(position.X - target_pos.X);
    int dist_y = std::abs(position.Y - target_pos.Y);

    if (dist_x > 0 && dist_y > 0)
    {
        return false;
    }

    int manhattan_dist = dist_x + dist_y;
    return manhattan_dist <= speed.get_speed();
}

void Unit::move_to(COORD new_pos)
{
    if (can_move_to(new_pos))
    {
        position = new_pos;
    }
}

