#pragma once
#include "SharkState.hpp"

class SharkRestState : public SharkState
{
public:
	void Execute(kmint::pigisland::shark* shark) override;
};


