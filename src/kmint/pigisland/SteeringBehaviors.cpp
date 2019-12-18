#include "..\..\..\include\kmint\pigisland\SteeringBehaviors.hpp"
#include <random>

SteeringBehaviors::SteeringBehaviors()
{
}

math::vector2d SteeringBehaviors::calculate()
{
	return Wander();
}

math::vector2d SteeringBehaviors::Wander()
{
	math::vector2d m_vWanderTarget;

	std::uniform_real_distribution<float> unif(-1, 1);
	std::default_random_engine re;
	float r1 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX / 2) - 1;
	float r2 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX / 2) - 1;
	m_vWanderTarget += math::vector2d(r1 * m_dWanderJitter,
		r2 * m_dWanderJitter);

	//reproject this new vector back onto a unit circle
	m_vWanderTarget = math::normalize(m_vWanderTarget);
	m_vWanderTarget *= m_dWanderRadius;
	math::vector2d targetLocal = m_vWanderTarget + math::vector2d(m_dWanderDistance, 0);
	return targetLocal;
}

math::vector2d SteeringBehaviors::Seek(const play::free_roaming_actor* evader, math::vector2d TargetPos)
{
	math::vector2d DesiredVelocity = math::normalize(TargetPos - evader->location())
		* evader->MaxSpeed();
	return (DesiredVelocity);
}

math::vector2d SteeringBehaviors::Flee(const play::actor* chaser, const play::free_roaming_actor* evader)
{
	math::vector2d DesiredVelocity = math::normalize(chaser->location() - evader->location());
	return (DesiredVelocity);
}
