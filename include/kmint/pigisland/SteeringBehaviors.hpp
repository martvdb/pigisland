#pragma once
#include "kmint/math/vector2d.hpp" // voor window en app
#include "kmint/play.hpp"
#include "kmint/play/free_roaming_actor.hpp"
#include  "kmint/random.hpp"
#include <vector>
using namespace kmint;
class SteeringBehaviors {
public:
	SteeringBehaviors(play::free_roaming_actor* steeringActor);
	math::vector2d calculate();
	math::vector2d Wander();
	math::vector2d Seek(math::vector2d target);
	math::vector2d Flee();
	math::vector2d Separation();
	math::vector2d Alignment();
	math::vector2d Cohesion();

	bool FleeOn() const;
	bool SeekOn() const;

	void setSeek(bool seek, math::vector2d chaseTarget);
	void setFlee(bool flee, math::vector2d fleeTarget);

	void CreateFeelers();

	math::vector2d Truncate(kmint::math::vector2d steerForce, float maxForce) const;


private:
	// rectangle_drawable drawable_;
	float m_dWanderRadius = 5;
	//This is the radius of the constraining circle.
	float m_dWanderDistance = 3.f;
	//This is the distance the wander circle is projected in front of the agent.
	float m_dWanderJitter = 28.f;

	std::vector<math::vector2d> m_Feelers;

	math::vector2d _fleeTarget{};
	math::vector2d _chaseTarget{};
	
	//SteeringBehaviorWeights
	float _dWanderAmount;
	float _dFleeAmount{};
	float _dSeekAmount{};
	float _dAlingmentAmount{};
	float _dCohesionAmount{};
	float _dSeparationAmount{};
	float _dWallAvoidanceAmount;
	bool isFleeOn = false;
	bool isSeekOn = false;
	
	play::free_roaming_actor* steeringActor{};
};