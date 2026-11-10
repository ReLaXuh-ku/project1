#pragma once
class Health
{
private:
    int value;

public:
    Health(int start_value) : value(start_value < 0 ? 0 : start_value) {}

    int get_health() const { return value; }

    void decrease(int amount)
    {
        if (amount > 0)
        {
            value -= amount;
            if (value < 0) value = 0;
        }
    }

    void increase(int amount)
    {
        if (amount > 0)
        {
            value += amount;
        }
    }

    bool is_zero() const { return value == 0; }
};

class Armor
{
private:
    int value;

public:
    Armor(int start_value) : value(start_value < 0 ? 0 : start_value) {}

    int get_armor() const { return value; }

    int reduce_damage(int damage) const
    {
        int final_damage = damage - value;
        return final_damage < 0 ? 0 : final_damage;
    }
};

class Attack
{
private:
    int value;

public:
    Attack(int damage) : value(damage < 0 ? 0 : damage) {}

    int get_damage() const { return value; }

    int calculate_damage(const Armor& target_armor) const
    {
        return target_armor.reduce_damage(value);
    }
};

class AttackRange
{
private:
    int value;

public:
    AttackRange(int start_range) : value(start_range < 1 ? 1 : start_range) {}

    int get_range() const { return value; }
};

class Speed
{
private:
    int value;
public:
    Speed(int start_speed) : value(start_speed < 1 ? 1 : start_speed) {}

    int get_speed() const { return value; }
};