#pragma once
#include "BoatState.hpp"
class BoatDockState : public BoatState
{
public:
	BoatDockState(kmint::pigisland::boat* boat);
	void Execute(kmint::pigisland::boat* boat) override;
};