#pragma once
#include "kmint/math/vector2d.hpp" // voor window en app
#include "kmint/play.hpp"
#include "kmint/play/free_roaming_actor.hpp"
#include  "kmint/random.hpp"
using namespace kmint;
class SteeringBehaviors {
public:
	SteeringBehaviors(const play::free_roaming_actor* steeringActor);
	kmint::math::vector2d calculate();
	kmint::math::vector2d Wander();
	kmint::math::vector2d Seek(math::vector2d TargetPos);
	kmint::math::vector2d Flee(const math::vector2d chaser);
	math::vector2d Separation();
	math::vector2d Alignment();
	math::vector2d Cohesion();

	bool FleeOn() const;
	bool SeekOn() const;

	void setSeek(bool seek);
	void setFlee(bool flee);

	kmint::math::vector2d Truncate(kmint::math::vector2d steerForce, kmint::math::vector2d maxForce) const;

private:
	// rectangle_drawable drawable_;
	float m_dWanderRadius = 50;
	//This is the radius of the constraining circle.
	float m_dWanderDistance = 1.f;
	//This is the distance the wander circle is projected in front of the agent.
	float m_dWanderJitter = 28.f;
	//SteeringBehaviorWeights
	float _dWanderAmount;
	float _dFleeAmount;
	float _dSeekAmount;
	float _dAlingmentAmount;
	float _dCohesionAmount;
	float _dSeparationAmount;
	float _dWallAvoidanceAmount;
	bool isFleeOn = false;
	bool isSeekOn = false;
	play::free_roaming_actor* steeringActor;
};