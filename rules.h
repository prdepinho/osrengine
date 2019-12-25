#pragma once

#include "terrain.h"
#include "dice.h"
#include <cmath>

enum class DamageType {
	Acid,
	Bludgeoning,
	Cold,
	Fire,
	Force,
	Lightning,
	Necrotic,
	Piercing,
	Poison,
	Psionic,
	Radiant,
	Slashing,
	Thunder
};

std::string get_damage_type_name(DamageType damage_type);

struct Damage {
	Dice dice;
	int bonus_damage;
	DamageType type;
};

struct Attack {
	int attack_bonus;
	Damage damage;
};

int ability_modifier(int ability);

class AttackContext {
public:
	AttackContext(Miniature *attacker, Miniature *target, Terrain *terrain, Attack *attack) :
		attacker(attacker), target(target), terrain(terrain), attack(attack) {}
	bool is_in_range();
	bool is_in_long_range();
	bool attack_has_advantage();
	bool attack_has_disadvantage();
	int roll_test(bool advantage=false, bool disadvantage=false);
	bool attack_is_crit(int roll);
	bool attack_is_fumble(int roll);
	int get_attack_bonuses();
	bool attack_hit(int attack_roll);
	int roll_damage(bool crit);
	int get_damage_bonuses();
	void deal_damage(int damage);

	std::vector<int> get_attack_rolls() { return attack_rolls; }
	std::vector<int> get_damage_rolls() { return damage_rolls; }
private:
	Miniature *attacker;
	Miniature *target;
	Terrain *terrain;
	Attack *attack;

	std::vector<int> attack_rolls;
	std::vector<int> damage_rolls;
};
