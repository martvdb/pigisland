#include "../../../../../../libkmint/include/kmint/random.hpp"
#include "../../../../../include/kmint/pigisland/boat.hpp"
#include "../../../../../include/kmint/pigisland/States/BoatStates/BoatDockState.hpp"
#include "kmint/pigisland/States/BoatStates/BoatWanderState.hpp"

BoatDockState::BoatDockState(kmint::pigisland::boat* boat)
{
	boat->set_tint(graphics::color(100, 100, 255, 100));
	boat->mooringPlace = pigisland::find_random_mooring_place(boat->graph()).node_id();
}


void BoatDockState::Execute(kmint::pigisland::boat* boat)
{
	if (boat->node().node_id() == boat->mooringPlace) {
		auto i = std::find_if(boat->docks_.begin(), boat->docks_.end(), [boat](auto& dock) {
			return dock.dockNode() == boat->mooringPlace;
			});
		i->repair(*boat);
		boat->setState(new BoatWanderState(boat));
	}
	else {
		int next_index = calculateRoute(boat->node().node_id(), boat->mooringPlace, boat->graph());
		boat->node(boat->node()[next_index].to());
	}
}
