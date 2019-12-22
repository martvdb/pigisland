#pragma once
#include "kmint/pigisland/shark.hpp"
namespace kmint {
	namespace pigisland {

		class SharkState
		{
		public:
			virtual void Execute(shark* shark) = 0;
		};
	}
}

