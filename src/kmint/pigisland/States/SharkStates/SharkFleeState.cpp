#include "kmint/pigisland/States/SharkStates/SharkRestState.hpp"
#include "kmint/pigisland/States/SharkStates/SharkFleeState.hpp"
#include "kmint/pigisland/States/SharkStates/SharkWanderState.hpp"
#include "kmint/pigisland/States/SharkStates/SharkChaseState.hpp"
#include "../../../../../include/kmint/pigisland/shark.hpp"


SharkFleeState::SharkFleeState(kmint::pigisland::shark* shark)
{
	shark->set_tint(kmint::graphics::color(100, 255, 100));
	wanderSteps = 0;
}

void SharkFleeState::Execute(pigisland::shark* shark)
{
	int next_index = kmint::random_int(0, shark->node().num_edges());
	shark->node(shark->node()[next_index].to());

	wanderSteps++;
	if(wanderSteps >= 10)
	{
		shark->setState(new SharkWanderState(shark));
	}
	if (shark->amountOfSteps_ >= 100)
	{
		shark->setState(new SharkRestState(shark));
	}
}