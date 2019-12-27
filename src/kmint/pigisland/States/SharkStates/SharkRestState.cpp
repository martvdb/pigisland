#include "kmint/pigisland/States/SharkStates/SharkRestState.hpp"
#include "kmint/pigisland/States/SharkStates/SharkWanderState.hpp"
#include "kmint/pigisland/States/SharkStates/SharkChaseState.hpp"
#include "kmint/pigisland/States/SharkStates/SharkFleeState.hpp"


SharkRestState::SharkRestState(kmint::pigisland::shark* shark)
{
	shark->set_tint(graphics::color(100, 100, 255));
}

void SharkRestState::Execute(pigisland::shark* shark)
{
	if (shark->node().node_id() == pigisland::find_shark_resting_place(shark->graph()).node_id()) {
		shark->amountOfSteps_ = 0;
		for (auto i = shark->begin_perceived(); i != shark->end_perceived(); ++i) {
			if (i->type() == "boat")
			{
				if (distance(i->location(), shark->location()) <= 50)
				{
					shark->setState(new SharkFleeState(shark));
					return;
				}

			}
			if (i->type() == "pig")
			{
				shark->setState(new SharkChaseState(shark));
				return;
			}
		}
		shark->setState(new SharkWanderState(shark));
	}
	else {
		int next_index = calculateRoute(shark->node().node_id(), pigisland::find_shark_resting_place(shark->graph()).node_id(), shark->graph());
		shark->node(shark->node()[next_index].to());
	}
}

