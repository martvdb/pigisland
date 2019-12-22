#pragma once
#include "BoatState.hpp"

class BoatWanderState : public BoatState
{
public:
	void Execute(kmint::pigisland::boat* boat) override;
};