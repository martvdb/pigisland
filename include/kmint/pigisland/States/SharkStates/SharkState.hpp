#pragma once
#include "kmint/pigisland/shark.hpp"
namespace kmint {
	namespace pigisland {
		class shark;
	}
}

class SharkState
{
public:
	virtual void Execute(kmint::pigisland::shark* shark) = 0;
};







