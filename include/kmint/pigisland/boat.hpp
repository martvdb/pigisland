#ifndef KMINT_PIGISLAND_BOAT_HPP
#define KMINT_PIGISLAND_BOAT_HPP
#pragma once
#include "kmint/map/map.hpp"
#include "kmint/play.hpp"
#include "kmint/primitives.hpp"
#include "kmint/pigisland/dock.hpp"
#include "kmint/pigisland/node_algorithm.hpp"
#include "kmint/pigisland/resources.hpp"
#include "kmint/random.hpp"
#include "kmint/pigisland/Aster.hpp"
#include "kmint/pigisland/pig.hpp"
#include "States/BoatStates/BoatState.hpp"

namespace kmint {
namespace pigisland {
class dock;
class boat : public play::map_bound_actor {
public:
	boat(map::map_graph& g, map::map_node& initial_node);
	// wordt elke game tick aangeroepen
	void act(delta_time dt) override;
	ui::drawable const& drawable() const override { return drawable_; }
	// als incorporeal false is, doet de actor mee aan collision detection
	bool incorporeal() const override { return false; }
	// geeft de lengte van een zijde van de collision box van deze actor terug.
	// Belangrijk voor collision detection
	scalar collision_range() const override { return 32.0; }
	std::string type() const override { return "boat"; }
	void repair(int amountOfRepairPoints);
	int paintDamage_ = 0;
	void setState(BoatState* state);
	BoatState* state() { return state_; }
	map::map_graph& graph() { return graph_; }
	int mooringPlace;
	std::vector<dock> docks_;
private:
	// hoeveel tijd is verstreken sinds de laatste beweging
	delta_time t_passed_{};
	
	// weet hoe de koe getekend moet worden
	play::image_drawable drawable_;
	BoatState* state_;
	map::map_graph &graph_;
	
};

} // namespace pigisland
} // namespace kmint

#endif /* KMINT_PIGISLAND_BOAT_HPP */
