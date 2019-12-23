#include "kmint/pigisland/pig.hpp"
#include "kmint/pigisland/resources.hpp"

namespace kmint::pigisland
{
pig::pig(math::vector2d location)
  : play::free_roaming_actor{location},
    drawable_{*this, pig_image()} , Steering{ SteeringBehaviors(this) }
{
	heading = math::normalize(math::vector2d(random_scalar(-1, 1), random_scalar(-1, 1)));
	side = perp(heading);
}

void pig::act(delta_time dt) {

	for (auto i = begin_perceived(); i != end_perceived(); ++i) {
		if (i->type() == "boat")
		{
			Steering.setSeek(true, i->location());
		}
		if (i->type() == "shark")
		{
			Steering.setFlee(true, i->location());
		}
	}


	steer_force = Steering.calculate();
	
	
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
} // namespace kmint
