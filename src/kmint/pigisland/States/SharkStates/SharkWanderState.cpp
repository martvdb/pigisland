#include "kmint/pigisland/States/SharkStates/SharkWanderState.hpp"
#include "../../../../../../libkmint/include/kmint/random.hpp"
#include "kmint/pigisland/States/SharkStates/SharkChaseState.hpp"
#include "kmint/pigisland/States/SharkStates/SharkRestState.hpp"
#include "kmint/pigisland/States/SharkStates/SharkFleeState.hpp"

SharkWanderState::SharkWanderState(kmint::pigisland::shark* shark)
{
	shark->set_tint(graphics::color(255, 255, 255, 255));
}

void SharkWanderState::Execute(pigisland::shark* shark)
{
	int next_index = kmint::random_int(0, shark->node().num_edges());
	shark->node(shark->node()[next_index].to());
	shark->amountOfSteps_++;
	if(shark->amountOfSteps_ >= 100)
	{
		shark->setState(new SharkRestState(shark));
		return;
	}
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
}
