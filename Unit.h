#pragma once
#include <Windows.h>
#include "attributes.h"
#include <memory>

enum UnitType {bruiser, samurai, mage, soldier, healer, cannoneer};
class Unit
{
protected:
	COORD position;
	Health health;
	Armor armor;
	Attack attack;
	AttackRange range;
	Speed speed;
	UnitType type;
public:
	Unit(COORD pos, int hp, int arm, int atk, int rng, int s, UnitType t);
	virtual ~Unit() = default;

	COORD get_position() const;
	bool is_alive() const;
	UnitType get_type() const;
	virtual std::unique_ptr<Unit> clone() const = 0;
	const Armor& get_armor() const { return armor; }
	int get_health_value() const { return health.get_health(); }
	int get_armor_value() const { return armor.get_armor(); }
	int get_attack_value() const { return attack.get_damage(); }
	int get_range_value() const { return range.get_range(); }
	int get_speed_value() const { return speed.get_speed(); }

	bool can_attack(COORD target_pos) const;
	void take_damage(int final_dmg);
	void take_healing(int amount);
	virtual void execute_action(Unit& target) = 0;
	bool can_move_to(COORD target_pos) const;
	void move_to(COORD new_pos);
};