#include "..\..\..\include\kmint\pigisland\SteeringBehaviors.hpp"
#include <random>
using namespace kmint;

SteeringBehaviors::SteeringBehaviors(play::free_roaming_actor* steeringActor) : _dWanderAmount { 0.7 } , _dWallAvoidanceAmount { 3 }
{
	this->steeringActor = steeringActor;
	_dFleeAmount = random_scalar(-1, 1);
	_dSeekAmount = random_scalar(-1, 1);
	_dCohesionAmount = random_scalar(0, 1);
	_dAlingmentAmount = random_scalar(0, 1);
	_dSeparationAmount = random_scalar(0, 1);
	m_Feelers(3);
}

math::vector2d SteeringBehaviors::calculate() {

	math::vector2d SteeringForce;
	SteeringForce += Wander() * _dWanderAmount;
	if (SeekOn()) { SteeringForce += Flee() * _dFleeAmount; }
	if (FleeOn()) { SteeringForce += Seek(_chaseTarget) * _dSeekAmount; }
	SteeringForce += Alignment() * _dAlingmentAmount;
	SteeringForce += Cohesion() * _dCohesionAmount;
	SteeringForce += Separation() * _dSeparationAmount;
	return Truncate(SteeringForce, 15);
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

math::vector2d SteeringBehaviors::Seek(math::vector2d target)
{
	math::vector2d DesiredVelocity = math::normalize(target - steeringActor->location())
		* steeringActor->MaxSpeed();
	return (DesiredVelocity);
}

math::vector2d SteeringBehaviors::Flee()
{
	math::vector2d DesiredVelocity = math::normalize(steeringActor->location() - _fleeTarget) * steeringActor->MaxSpeed();
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

void SteeringBehaviors::CreateFeelers()
{
	//feeler pointing straight in front
	m_Feelers[0] = steeringActor->location() + 10 * steeringActor->Heading();

	//feeler to left
	math::vector2d temp = steeringActor->Heading();
	temp.x(temp.y());
	temp.y(-temp.x());
	m_Feelers[1] = steeringActor->location() + 10 / 2.0f * temp;

	//feeler to right
	temp = steeringActor->Heading();
	temp.x(-temp.y());
	temp.y(temp.x());
	m_Feelers[2] = steeringActor->location() + 10 / 2.0f * temp;
}


bool SteeringBehaviors::FleeOn() const
{
	return isFleeOn;
}
bool SteeringBehaviors::SeekOn() const
{
	return isSeekOn;
}

void SteeringBehaviors::setFlee(bool flee, math::vector2d fleeTarget)
{
	_fleeTarget = fleeTarget;
	isFleeOn = flee;
}
void SteeringBehaviors::setSeek(bool seek, math::vector2d chaseTarget)
{
	_chaseTarget = chaseTarget;
	isSeekOn = seek;
}

math::vector2d SteeringBehaviors::Truncate(math::vector2d steerForce, float maxForce) const
{
	if (length(steerForce) > maxForce)
	{
		steerForce = normalize(steerForce) * maxForce;
	}
	return  steerForce;
}

