#ifndef KMINT_PIGISLAND_BOAT_HPP
#define KMINT_PIGISLAND_BOAT_HPP
#pragma once
#include "kmint/map/map.hpp"
#include "kmint/play.hpp"
#include "kmint/primitives.hpp"
#include "kmint/pigisland/dock.hpp"

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
	scalar collision_range() const override { return 16.0; }
	void repair(int amountOfRepairPoints);
private:
	// hoeveel tijd is verstreken sinds de laatste beweging
	delta_time t_passed_{};
	std::vector<dock> docks_;
	// weet hoe de koe getekend moet worden
	play::image_drawable drawable_;
	int paintDamage_ = 0;
	map::map_graph &graph_;
	int mooringPlace;
};

} // namespace pigisland
} // namespace kmint

#endif /* KMINT_PIGISLAND_BOAT_HPP */
