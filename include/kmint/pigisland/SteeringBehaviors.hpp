#pragma once
#include "kmint/math/vector2d.hpp" // voor window en app
#include "kmint/play.hpp"
#include "kmint/play/free_roaming_actor.hpp"
using namespace kmint;
class SteeringBehaviors {
public:
	SteeringBehaviors();
	kmint::math::vector2d calculate();
	kmint::math::vector2d Wander();
	kmint::math::vector2d Seek(const play::free_roaming_actor* seeker, math::vector2d TargetPos);
	kmint::math::vector2d Flee(const play::actor* chaser, const play::free_roaming_actor* evader);
private:
	// rectangle_drawable drawable_;
	float m_dWanderRadius = 2500;
	//This is the radius of the constraining circle.
	float m_dWanderDistance = 5.f;
	//This is the distance the wander circle is projected in front of the agent.
	float m_dWanderJitter = 28.f;
};