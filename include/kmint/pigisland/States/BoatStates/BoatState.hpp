#pragma once
#include "kmint/pigisland/boat.hpp"
namespace kmint {
	namespace pigisland {
		class boat;
	}
}

class BoatState
{
public:
	virtual void Execute(kmint::pigisland::boat* boat) = 0;
};







