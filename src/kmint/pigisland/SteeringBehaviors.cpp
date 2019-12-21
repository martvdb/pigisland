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

math::vector2d SteeringBehaviors::Flee(const math::vector2d chaser, const play::free_roaming_actor* evader)
{
	math::vector2d DesiredVelocity = math::normalize(evader->location() - chaser) * evader->MaxSpeed();
	return (DesiredVelocity);
}

math::vector2d SteeringBehaviors::Separation(const play::free_roaming_actor* separator)
{
	math::vector2d SteeringForce;
	int NeighborCount = 0;
	for (auto i = separator->begin_perceived(); i != separator->end_perceived(); ++i)
	{
		auto const& a = *i;
		
		//make sure this agent isn't included in the calculations and that
		//the agent being examined is close enough.
		if (a.type() == "pig")
		{
			math::vector2d ToAgent = separator->location() - a.location();
			//scale the force inversely proportional to the agent's distance
			//from its neighbor.
			SteeringForce += normalize(ToAgent) / length(ToAgent);
			++NeighborCount;
		}
	}
	//if (NeighborCount > 0)
	//{
	//	SteeringForce /= (double)NeighborCount;
	//}
	return SteeringForce;
}

math::vector2d SteeringBehaviors::Alignment(const play::free_roaming_actor* aligner)
{
	//used to record the average heading of the neighbors
	math::vector2d AverageHeading;
	//used to count the number of vehicles in the neighborhood
	int NeighborCount = 0;
		//iterate through all the tagged vehicles and sum their heading vectors
		for (auto i = aligner->begin_perceived(); i != aligner->end_perceived(); ++i)
		{
			auto const& a = *i;

			//make sure this agent isn't included in the calculations and that
			//the agent being examined is close enough.
			if (a.type() == "pig")
			{
				AverageHeading += a.heading();
				++NeighborCount;
			}
		}
	//if the neighborhood contained one or more vehicles, average their
	//heading vectors.
	if (NeighborCount > 0)
	{
		AverageHeading /= (double)NeighborCount;
		AverageHeading -= aligner->Heading();
	}
	return AverageHeading;
}

math::vector2d SteeringBehaviors::Cohesion(const play::free_roaming_actor* aligner)
{
	//first find the center of mass of all the agents
	math::vector2d CenterOfMass, SteeringForce;
	int NeighborCount = 0;
	//iterate through the neighbors and sum up all the position vectors
	for (auto i = aligner->begin_perceived(); i != aligner->end_perceived(); ++i)
	{
		auto const& a = *i;

		//make sure this agent isn't included in the calculations and that
		//the agent being examined is close enough.
		if (a.type() == "pig")
		{
			CenterOfMass += a.location();
			++NeighborCount;
		}
	}
	if (NeighborCount > 0)
	{
		//the center of mass is the average of the sum of positions
		CenterOfMass /= (double)NeighborCount;
		//now seek toward that position
		SteeringForce = Seek(aligner, CenterOfMass);
	}
	return normalize(SteeringForce);
}


