#pragma once
#include "SharkState.hpp"

class SharkFleeState : public SharkState
{
public:
	SharkFleeState(kmint::pigisland::shark* shark);
	void Execute(kmint::pigisland::shark* shark) override;
private:
	int wanderSteps;
};