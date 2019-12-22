#include "kmint/pigisland/States/SharkStates/SharkWanderState.hpp"
#include "../../../../../../libkmint/include/kmint/random.hpp"
#include "kmint/pigisland/States/SharkStates/SharkChaseState.hpp"
#include "kmint/pigisland/States/SharkStates/SharkRestState.hpp"

void SharkWanderState::Execute(pigisland::shark* shark)
{
	int next_index = kmint::random_int(0, shark->node().num_edges());
	shark->node(shark->node()[next_index].to());
	shark->amountOfSteps_++;
	if(shark->amountOfSteps_ >= 100)
	{
		shark->setState(new SharkRestState());
		return;
	}
	if (shark->num_perceived_actors() > 0)
	{
		shark->setState(new SharkChaseState());
	}
}
