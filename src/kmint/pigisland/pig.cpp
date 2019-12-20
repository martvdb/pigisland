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

	steer_force = normalize(Steering.Wander()) * 25;
	
	for (auto i = begin_perceived(); i != end_perceived(); ++i) {
		if (i->type() == "boat")
		{
			steer_force = Steering.Seek(this, i->location()) * 10;
		}
		if(i->type() == "shark")
		{
			steer_force = Steering.Flee(i->location(), this) * 10;
		}
	}
	
	steer_force += Steering.Cohesion(this) * 1;
	steer_force += Steering.Separation(this) * 1;
	steer_force += Steering.Alignment(this) * 3;

	if(length(steer_force) > maxForce)
	{
		steer_force = normalize(steer_force) * maxForce;
	}
	
	
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
