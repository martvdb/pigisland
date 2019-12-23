#include "../../../../../../libkmint/include/kmint/random.hpp"
#include "../../../../../include/kmint/pigisland/boat.hpp"
#include "../../../../../include/kmint/pigisland/States/BoatStates/BoatDockState.hpp"
#include "../../../../../include/kmint/pigisland/States/BoatStates/BoatWanderState.hpp"

BoatWanderState::BoatWanderState(kmint::pigisland::boat* boat)
{
	boat->set_tint(graphics::color(255, 255, 255, 255));
}

void BoatWanderState::Execute(kmint::pigisland::boat* boat)
{
	int next_index = kmint::random_int(0, boat->node().num_edges());
	boat->node(boat->node()[next_index].to());
	for (auto i = boat->begin_collision(); i != boat->end_collision(); ++i) {
		if (i->type() == "pig") {
			i->remove();
		}
	}
	boat->paintDamage_++;
	if (boat->paintDamage_ >= 100) {
		boat->setState(new BoatDockState(boat));
	}
}
