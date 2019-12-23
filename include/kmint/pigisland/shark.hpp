#ifndef KMINT_PIGISLAND_SHARK_HPP
#define KMINT_PIGISLAND_SHARK_HPP

#include "kmint/map/map.hpp"
#include "kmint/play.hpp"
#include "kmint/primitives.hpp"
#include "kmint/pigisland/node_algorithm.hpp"
#include "kmint/pigisland/resources.hpp"
#include "kmint/random.hpp"
#include "kmint/pigisland/Aster.hpp"
#include "kmint/pigisland/pig.hpp"
#include "States/SharkStates/SharkState.hpp"
class SharkState;
namespace kmint {
namespace pigisland {

class shark : public play::map_bound_actor {
public:
	shark(map::map_graph& g, map::map_node& initial_node);
	// wordt elke game tick aangeroepen
	void act(delta_time dt) override;
	ui::drawable const& drawable() const override { return drawable_; }
	// als incorporeal false is, doet de actor mee aan collision detection
	bool incorporeal() const override { return false; }
	// geeft de lengte van een zijde van de collision box van deze actor terug.
	// Belangrijk voor collision detection
	scalar collision_range() const override { return 32.0; }
	bool perceptive() const override { return true; }
	scalar perception_range() const override { return 500.0f; }
	std::string type() const override { return "shark"; }
	bool perceivable() const override { return true; }
	void setState(SharkState* state);
	SharkState* state() { return state_; }
	void set_tint(graphics::color color) { drawable_.set_tint(color); }
	int amountOfSteps_ = 0;
	map::map_graph& graph() { return graph_; }
private:
	SharkState* state_;
	
	// hoeveel tijd is verstreken sinds de laatste beweging
	delta_time t_passed_{};
	// weet hoe de koe getekend moet worden
	play::image_drawable drawable_;
	map::map_graph &graph_;
};

} // namespace pigisland
} // namespace kmint

#endif /* KMINT_PIGISLAND_SHARK_HPP */
