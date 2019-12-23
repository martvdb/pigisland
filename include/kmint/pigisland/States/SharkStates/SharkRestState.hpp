#pragma once
#include "SharkState.hpp"

class SharkRestState : public SharkState
{
public:
	SharkRestState(kmint::pigisland::shark* shark);
	void Execute(kmint::pigisland::shark* shark) override;
};


