#pragma once
#include "BoatState.hpp"

class BoatWanderState : public BoatState
{
public:
	BoatWanderState(kmint::pigisland::boat* boat);
	void Execute(kmint::pigisland::boat* boat) override;
};