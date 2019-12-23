#include "..\..\..\include\kmint\pigisland\SteeringBehaviors.hpp"
#include <random>
using namespace kmint;

SteeringBehaviors::SteeringBehaviors(play::free_roaming_actor* steeringActor) : _dWanderAmount{ 0.5 }, _dWallAvoidanceAmount{ 3 }
{
	this->steeringActor = steeringActor;
	_dFleeAmount = random_scalar(-1, 1);
	_dSeekAmount = random_scalar(-1, 1);
	_dCohesionAmount = random_scalar(0, 1);
	_dAlingmentAmount = random_scalar(0, 1);
	_dSeparationAmount = random_scalar(0, 1);
	m_Walls.push_back(math::line_segment(math::vector2d(0, 0), math::vector2d(0, 768)));
	m_Walls.push_back(math::line_segment(math::vector2d(0, 0), math::vector2d(1024, 0)));
	m_Walls.push_back(math::line_segment(math::vector2d(0, 768), math::vector2d(1024, 768)));
	m_Walls.push_back(math::line_segment(math::vector2d(1024, 0), math::vector2d(1024, 768)));

}

math::vector2d SteeringBehaviors::calculate() {

	math::vector2d SteeringForce;
	SteeringForce += Wander() * _dWanderAmount;
	if (SeekOn()) { SteeringForce += Flee() * _dFleeAmount; isFleeOn = false; }
	if (FleeOn()) { SteeringForce += Seek(_chaseTarget) * _dSeekAmount; isSeekOn = false; }
	SteeringForce += Alignment() * _dAlingmentAmount;
	SteeringForce += Cohesion() * _dCohesionAmount;
	SteeringForce += Separation() * _dSeparationAmount;
	SteeringForce += WallAvoidance() * _dWallAvoidanceAmount;
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
			SteeringForce += ToAgent / length(ToAgent);
			++NeighborCount;
		}
	}
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
	return SteeringForce;
}

void SteeringBehaviors::CreateFeelers()
{
	m_Feelers = std::vector<math::vector2d>();
	//feeler pointing straight in front
	m_Feelers.push_back(steeringActor->location() + steeringActor->Heading() * 100);

	//feeler to left
	math::vector2d temp = steeringActor->Heading() * 50;
	float x = temp.x();
	temp.x(temp.y());
	temp.y(-x);
	m_Feelers.push_back(steeringActor->location() + temp);

	//feeler to right
	temp = steeringActor->Heading() * 50;
	x = temp.x();
	temp.x(-temp.y());
	temp.y(x);
	m_Feelers.push_back(steeringActor->location() + temp);
}

math::vector2d SteeringBehaviors::WallAvoidance() {

	CreateFeelers();
	
	double DistToThisIP = 0.0;
	double DistToClosestIP = 10000;
	//this will hold an index into the vector of walls
	int ClosestWall = -1;
	math::vector2d SteeringForce, point, //used for storing temporary info
		ClosestPoint; //holds the closest intersection point
		//examine each feeler in turn
	for (int flr = 0; flr < m_Feelers.size(); ++flr) { //run through each wall checking for any intersection points
		for (int w = 0; w < m_Walls.size(); ++w) {
			math::line_segment vehicleFeelerSegment = math::line_segment(steeringActor->location(), m_Feelers[flr]);
			if (intersect(vehicleFeelerSegment, m_Walls[w]))
			{ //is this the closest found so far? If so keep a record
				if (DistToThisIP < DistToClosestIP)
				{
					DistToClosestIP = DistToThisIP;
					ClosestWall = w;
					ClosestPoint = point;
				}
			}
		}//next wall
	//if an intersection point has been detected, calculate a force //that will direct the agent away
		if (ClosestWall >= 0)
		{
			//calculate by what distance the projected position of the agent
			//will overshoot the wall
			math::vector2d OverShoot = m_Feelers[flr] - ClosestPoint;
			//create a force in the direction of the wall normal, with a
			//magnitude of the overshoot
			//SteeringForce = m_Walls[ClosestWall].Normal() * OverShoot.Length();
		}
	}//next feeler
	return SteeringForce;
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

