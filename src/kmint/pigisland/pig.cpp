#include "kmint/pigisland/pig.hpp"
#include "kmint/pigisland/resources.hpp"
#include "kmint/random.hpp"

namespace kmint {
namespace pigisland {


pig::pig(math::vector2d location)
  : play::free_roaming_actor{location},
    drawable_{*this, pig_image()} {
	Steering = SteeringBehaviors();
	knabbelFactor_ = random_scalar(-1, 1);
	boatFactor_ = random_scalar(-1, 1);
	cohesionFactor_ = random_scalar(0, 1);
	alignmentFactor_ = random_scalar(0, 1);
	separationFactor_ = random_scalar(0, 1);
}

void pig::act(delta_time dt) {

	steer_force = normalize(Steering.Wander());
	
	for (auto i = begin_perceived(); i != end_perceived(); ++i) {
		if (i->type() == "boat")
		{
			steer_force = Steering.Seek(this, i->location()) * boatFactor_;
		}
		if(i->type() == "shark")
		{
			steer_force = Steering.Flee(i->location(), this) * knabbelFactor_;
		}
	}
	steer_force += Steering.Separation(this) * separationFactor_;
	steer_force += Steering.Cohesion(this) * cohesionFactor_;
	steer_force += Steering.Alignment(this) * alignmentFactor_;

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
