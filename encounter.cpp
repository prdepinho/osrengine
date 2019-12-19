#include "encounter.h"
#include "dice.h"
#include <algorithm>

void Encounter::roll_initiative() {
	std::vector<Miniature*> minies = terrain->get_minies();

	initiative_order = std::vector<Initiative>(minies.size());
	for (int i = 0; i < minies.size(); i++) {
		initiative_order[i] = Initiative{ minies[i], minies[i]->get_character().initiative + roll_die(20) };
	}

	std::sort(
			initiative_order.begin(),
		   	initiative_order.end(),
		   	[&](Initiative a, Initiative b) {
				return a.initiative < b.initiative;
			});
}

Miniature *Encounter::next_mini() {
	if (initiative_order.size() == 0)
		return nullptr;
	Miniature *mini = initiative_order.back().mini;
	initiative_order.pop_back();
	return mini;
}
