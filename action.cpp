#include "action.h"
#include "rules.h"
#include "dice.h"

void EchoAction::effect(Terrain &terrain, Miniature &actor, Coordinates target) {
	std::stringstream ss;
	ss << "Actor: " << actor.get_x() << ", " << actor.get_y() << "; ";
	ss << "Target: " << target.x << ", " << target.y << ".";
	Log::write(ss.str());
}

void AttackAction::effect(Terrain &terrain, Miniature &actor, Coordinates coords_target) {
	Log::write("Attack action");
	auto minies = terrain.get_minies(coords_target);
	for (Miniature *target : minies) {
		Attack attack{+0, Damage{Dice(1, 4), +0, DamageType::Bludgeoning}};
		AttackContext context(&actor, target, &terrain, &attack);

		/*
		 * Foo: attack roll: 16 + 4 = 20 HIT!.
		 * Bar: received 8 bludgeoning damage from Foo.
		 *
		 *
		 * Foo: attack: 16 + 4 = 20 HIT! Caused 8 bludgeoning damage to Bar.
		 * Foo: attack: 3 + 4 = 7 MISS!
		 * Foo: attack: Adv. (16, 7) + 4 = 20 HIT! Caused 8 bludgeoning damage to Bar.
		 * Foo: attack: 20 CRIT! Caused 16 bludgeoning damage to Bar.
		 * Foo: attack: Disadv. (1, 17) FUMBLE!
		 */
		if (context.is_in_range()) {
			bool advantage = context.attack_has_advantage();
			bool disadvantage = context.attack_has_disadvantage();
			int attack_roll = context.roll_test(advantage, disadvantage);
			std::vector<int> attack_rolls = context.get_attack_rolls();
			bool is_crit = context.attack_is_crit(attack_roll);
			bool is_fumble = context.attack_is_fumble(attack_roll);
			int attack_bonuses = context.get_attack_bonuses();
			int attack_total = attack_roll + attack_bonuses;
			bool attack_hit = context.attack_hit(attack_total);
			int damage_roll = 0;
			std::vector<int> damage_rolls;
			int damage_bonuses = 0;
			int damage_total = 0;
			if (attack_hit) {
				damage_roll = context.roll_damage(is_crit);
				damage_rolls = context.get_damage_rolls();
				damage_bonuses = context.get_damage_bonuses();
				damage_total = damage_roll + damage_bonuses;
				context.deal_damage(damage_total);
			}

			// log
			{
				std::stringstream ss;
				ss << actor.get_character().name << ": attack: ";
				if (advantage) {
					ss << "Adv. (" << attack_rolls[0] << ", " << attack_rolls[1] <<  ")";
					ss << " + " << attack_bonuses << " = " << attack_total;
				}
				else if (disadvantage) {
					ss << "Disadv. (" << attack_rolls[0] << ", " << attack_rolls[1] <<  ")";
					ss << " + " << attack_bonuses << " = " << attack_total;
				}
				else {
					ss << attack_roll << " + " << attack_bonuses << " = " << attack_total;
				}
				ss << " ";
				if (attack_hit) {
					ss << (is_crit ? "CRIT!" : "HIT!");
					ss << " Caused " << damage_total << " " << get_damage_type_name(attack.damage.type);
					ss << " damage to " << target->get_character().name;
				}
				else {
					ss << (is_fumble ? "FUMBLE!" : "MISS!");
				}
				Log::write(ss.str());

				if (target->get_character().cur_hp <= 0) {
					std::stringstream ss;
					ss << target->get_character().name << ": DEAD";
					Log::write(ss.str());
				}
			}
		}
	}
}
