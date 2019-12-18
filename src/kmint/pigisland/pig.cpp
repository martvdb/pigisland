#include "kmint/pigisland/pig.hpp"
#include "kmint/pigisland/resources.hpp"
#include "kmint/random.hpp"

namespace kmint {
namespace pigisland {


pig::pig(math::vector2d location)
  : play::free_roaming_actor{location},
    drawable_{*this, pig_image()} {
	Steering = SteeringBehaviors();
}

void pig::act(delta_time dt) {


	for (auto i = begin_perceived(); i != end_perceived(); ++i) {
		//const map::map_node& node = find_closest_node_to(graph_, i->location());
		//int node_id = node.node_id();
		//next_index = calculateRoute(this->node().node_id(), node_id, graph_);
	}
	steer_force = Steering.Wander();
	const kmint::math::vector2d acceleration = steer_force / mass;
	velocity += acceleration * kmint::to_seconds(dt);

	if (velocity.x() > maxSpeed) {
		velocity = { trunc(maxSpeed), velocity.y() };
	}
	if (velocity.y() > maxSpeed) {
		velocity = { velocity.y(), trunc(maxSpeed) };
	}

	if (length(velocity) > 0.00000001) {
		heading = normalize(velocity);
		side = perp(heading);
	}
	kmint::math::vector2d loc = location();
	location(location() + (velocity * kmint::to_seconds(dt)));
}


} // namespace pigisland

} // namespace kmint
