#include "kmint/pigisland/States/SharkStates/SharkWanderState.hpp"
#include "../../../../../../libkmint/include/kmint/random.hpp"
#include "kmint/pigisland/States/SharkStates/SharkChaseState.hpp"

void kmint::pigisland::SharkWanderState::Execute(shark* shark)
{
	for (auto i = shark->begin_perceived(); i != shark->end_perceived(); ++i) {
		//shark->setState(new SharkChaseState());
	}
	shark->next_index = kmint::random_int(0, shark->node().num_edges());

}
