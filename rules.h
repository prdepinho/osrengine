#pragma once

#include "terrain.h"
#include "dice.h"
#include <cmath>

enum class DamageType {
};

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
	bool attack_has_advantage();
	bool attack_has_disadvantage();
	int roll_test(bool advantage=false, bool disadvantage=false);
	bool attack_is_crit(int roll);
	bool attack_is_fumble(int roll);
	int add_attack_bonuses(int attack_roll);
	bool attack_hit(int attack_roll);
	int roll_damage(bool crit);
	int add_damage_bonuses(int damage_roll);
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
