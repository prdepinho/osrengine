#include "rules.h"
#include <algorithm>

std::string get_damage_type_name(DamageType damage_type) {
	switch(damage_type) {
		case DamageType::Acid: return "Acid";
		case DamageType::Bludgeoning: return "Bludgeoning";
		case DamageType::Cold: return "Cold";
		case DamageType::Fire: return "Fire";
		case DamageType::Force: return "Force";
		case DamageType::Lightning: return "Lightning";
		case DamageType::Necrotic: return "Necrotic";
		case DamageType::Piercing: return "Piercing";
		case DamageType::Poison: return "Poison";
		case DamageType::Psionic: return "Psionic";
		case DamageType::Radiant: return "Radiant";
		case DamageType::Slashing: return "Slashing";
		case DamageType::Thunder: return "Thunder";
	}
}

inline int ability_modifier(int ability) {
	return std::floor((ability - 10) / 2);
}

bool AttackContext::is_in_range() {
	return true;
}

bool AttackContext::is_in_long_range() {
	return false;
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

bool AttackContext::attack_is_crit(int roll) {
	return roll == 20;
}

bool AttackContext::attack_is_fumble(int roll) {
	return roll == 1;
}

int AttackContext::get_attack_bonuses() {
	int proficiency_bonus = attacker->get_character().prof_bonus;
	int ability_bonus = ability_modifier(attacker->get_character().abl_str);
	int weapon_bonus = attack->attack_bonus;
	return proficiency_bonus + ability_bonus + weapon_bonus;
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

int AttackContext::get_damage_bonuses() {
	int ability_bonus = ability_modifier(attacker->get_character().abl_str);
	int weapon_bonus = attack->damage.bonus_damage;
	return ability_bonus + weapon_bonus;
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
