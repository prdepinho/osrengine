#include "rules.h"
#include <algorithm>

inline int ability_modifier(int ability) {
	return std::floor((ability - 10) / 2);
}

bool AttackContext::attack_has_advantage() {
	return false;
}

bool AttackContext::attack_has_disadvantage() {
	return false;
}

int AttackContext::roll_test(bool advantage, bool disadvantage) {
	attack_rolls.clear();
	if (advantage && !disadvantage) {
		attack_rolls.push_back(roll_die(20));
		attack_rolls.push_back(roll_die(20));
		return std::max(attack_rolls[0], attack_rolls[1]);
	}
	else if (!advantage && disadvantage) {
		attack_rolls.push_back(roll_die(20));
		attack_rolls.push_back(roll_die(20));
		return std::min(attack_rolls[0], attack_rolls[1]);
	}
	else {
		attack_rolls.push_back(roll_die(20));
		return attack_rolls[0];
	}
}

inline bool AttackContext::attack_is_crit(int roll) {
	return roll == 20;
}

inline bool AttackContext::attack_is_fumble(int roll) {
	return roll == 1;
}

int AttackContext::add_attack_bonuses(int attack_roll) {
	int proficiency_bonus = attacker->get_character().prof_bonus;
	int ability_bonus = ability_modifier(attacker->get_character().abl_str);
	int weapon_bonus = attack->attack_bonus;
	return attack_roll + proficiency_bonus + ability_bonus + weapon_bonus;
}

bool AttackContext::attack_hit(int attack_roll) {
	int ac = target->get_character().armor_class;
	return attack_roll >= ac;
}

int AttackContext::roll_damage(bool crit) {
	damage_rolls.clear();
	if (crit) {
		damage_rolls.push_back(attack->damage.dice.roll());
		damage_rolls.push_back(attack->damage.dice.roll());
		return damage_rolls[1] + damage_rolls[0];
	}
	damage_rolls.push_back(attack->damage.dice.roll());
	return damage_rolls[0];
}

int AttackContext::add_damage_bonuses(int damage_roll) {
	int ability_bonus = ability_modifier(attacker->get_character().abl_str);
	int weapon_bonus = attack->damage.bonus_damage;
	return damage_roll + ability_bonus + weapon_bonus;
}

void AttackContext::deal_damage(int damage) {
	target->get_character().cur_hp -= damage;
	if (target->get_character().cur_hp > 0) {
		if (damage > (target->get_character().max_hp / 2)) {
			// shock
		}
	}
	else {
		// dead
	}
}
