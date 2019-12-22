#pragma once
#include "SharkState.hpp"
#include "kmint/pigisland/shark.hpp"

class SharkChaseState : public SharkState
{
public:
	void Execute(pigisland::shark* shark) override;
};



