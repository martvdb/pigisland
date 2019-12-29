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
		int next_index = calculateRoute(shark->node().node_id(), pigisland::find_shark_resting_place(shark->graph()).node_id(), shark->graph());
		shark->node(shark->node()[next_index].to());

		if (shark->node().node_id() == pigisland::find_shark_resting_place(shark->graph()).node_id()) {
			shark->amountOfSteps_ = 0;

			shark->stage().setActorsInactive();
			auto locs = pigisland::random_pig_locations(100);
			for (auto loc : locs) {
				shark->stage().build_actor<pigisland::pig>(loc);
			}			
			shark->setState(new SharkWanderState(shark));
		}
}

