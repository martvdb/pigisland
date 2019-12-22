#include "kmint/pigisland/States/SharkStates/SharkRestState.hpp"
#include "kmint/pigisland/States/SharkStates/SharkWanderState.hpp"
#include "kmint/pigisland/States/SharkStates/SharkChaseState.hpp"


void SharkRestState::Execute(pigisland::shark* shark)
{
	if (shark->node().node_id() == pigisland::find_shark_resting_place(shark->graph()).node_id()) {
		shark->amountOfSteps_ = 0;
		if(shark->num_perceived_actors() > 0)
		{
			shark->setState(new SharkChaseState());
		} else
		{
			shark->setState(new SharkWanderState());
		}
		
		
	}
	else {
		int next_index = calculateRoute(shark->node().node_id(), pigisland::find_shark_resting_place(shark->graph()).node_id(), shark->graph());
		shark->node(shark->node()[next_index].to());
	}
}

