#include "kmint/pigisland/States/SharkStates/SharkChaseState.hpp"
#include "kmint/pigisland/States/SharkStates/SharkWanderState.hpp"
#include "kmint/pigisland/States/SharkStates/SharkRestState.hpp"


void SharkChaseState::Execute(pigisland::shark* shark)
{
	for (auto i = shark->begin_collision(); i != shark->end_collision(); ++i) {
		if (i->type() == "pig") {
			i->remove();
		}
	}
	float dist = 10000.0f;
	kmint::math::vector2d loc = kmint::math::vector2d();
	for (auto i = shark->begin_perceived(); i != shark->end_perceived(); ++i) {
		if (i->type() == "pig")
		{
			const map::map_node& node = pigisland::find_closest_node_to(shark->graph(), i->location());
			if (node.node_info().kind == 'W') {
				auto const& a = *i;
				if (distance(a.location(), shark->node().location()) < dist) {
					dist = distance(a.location(), shark->node().location());

					loc = a.location();
				}
			}
		}
	}
	if (loc != kmint::math::vector2d(0, 0)) {
		const map::map_node& node = pigisland::find_closest_node_to(shark->graph(), loc);

		int node_id = node.node_id();
		if (node_id != shark->node().node_id()) {
			int next_index = calculateRoute(shark->node().node_id(), node_id, shark->graph());
			shark->node(shark->node()[next_index].to());
		}
		shark->amountOfSteps_++;
		if(shark->amountOfSteps_ >= 100)
		{
			shark->setState(new SharkRestState());
		}
	}
	else
	{
		shark->setState(new SharkWanderState());
		shark->state()->Execute(shark);
	}
}

