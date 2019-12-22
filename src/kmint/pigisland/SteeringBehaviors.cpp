#include "..\..\..\include\kmint\pigisland\SteeringBehaviors.hpp"

SteeringBehaviors::SteeringBehaviors(play::free_roaming_actor* steeringActor) : _dWanderAmount { 1 } , _dWallAvoidanceAmount { 3 }
{
	this.steeringActor = steeringActor;
	_dFleeAmount = random_scalar(-1, 1);
	_dSeekAmount = random_scalar(-1, 1);
	_dCohesionAmount = random_scalar(0, 1);
	_dAlingmentAmount = random_scalar(0, 1);
	_dSeparationAmount = random_scalar(0, 1);
}

SVector2D SteeringBehaviors::calculate() {

	kmint::math::vector2d SteeringForce;
	SteeringForce += Wander() * dWanderAmount;
	if (SeekOn()) { SteeringForce += Flee() * dFleeAmount; }
	if (FleeOn()) { SteeringForce += Seek() * dSeekAmount; }
	SteeringForce += Alignment() * dAlingmentAmount;
	SteeringForce += Cohesion() * dCohesionAmount;
	SteeringForce += Separation() * dSeparationAmount;
	return Truncate(SteeringForce, 10);
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

math::vector2d SteeringBehaviors::Seek(math::vector2d TargetPos)
{
	math::vector2d DesiredVelocity = math::normalize(steeringActor->location() - TargetPos)
		* evader->MaxSpeed();
	return (DesiredVelocity);
}

math::vector2d SteeringBehaviors::Flee(const math::vector2d chaser)
{
	math::vector2d DesiredVelocity = math::normalize(steeringActor->location() - chaser) * evader->MaxSpeed();
	return (DesiredVelocity);
}

math::vector2d SteeringBehaviors::Separation()
{
	math::vector2d SteeringForce;
	int NeighborCount = 0;
	for (auto i = steeringActor->begin_perceived(); i != steeringActor->end_perceived(); ++i)
	{
		auto const& a = *i;
		
		//make sure this agent isn't included in the calculations and that
		//the agent being examined is close enough.
		if (a.type() == "pig")
		{
			math::vector2d ToAgent = steeringActor->location() - a.location();
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

math::vector2d SteeringBehaviors::Alignment()
{
	//used to record the average heading of the neighbors
	math::vector2d AverageHeading;
	//used to count the number of vehicles in the neighborhood
	int NeighborCount = 0;
		//iterate through all the tagged vehicles and sum their heading vectors
		for (auto i = steeringActor->begin_perceived(); i != steeringActor->end_perceived(); ++i)
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
		AverageHeading -= steeringActor->Heading();
	}
	return AverageHeading;
}

math::vector2d SteeringBehaviors::Cohesion()
{
	//first find the center of mass of all the agents
	math::vector2d CenterOfMass, SteeringForce;
	int NeighborCount = 0;
	//iterate through the neighbors and sum up all the position vectors
	for (auto i = steeringActor->begin_perceived(); i != steeringActor->end_perceived(); ++i)
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
		SteeringForce = Seek(CenterOfMass);
	}
	return normalize(SteeringForce);
}

bool SteeringBehaviors::FleeOn() const
{
	return isFleeOn;
}
bool SteeringBehaviors::SeekOn() const
{
	return isSeekOn;
}

void SteeringBehaviors::setFlee(bool flee)
{
	isFleeOn = flee;
}
void SteeringBehaviors::setSeek(bool seek)
{
	isSeekOn = seek;
}

kmint::math::vector2d SteeringBehaviors::Truncate(kmint::math::vector2d steerForce, kmint::math::vector2d maxForce) const
{
	if (length(steerForce) > maxForce)
	{
		steerForce = normalize(steerForce) * maxForce;
	}
	return  steerForce;
}

