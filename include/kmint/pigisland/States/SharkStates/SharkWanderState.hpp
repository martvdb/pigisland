#pragma once
#include "SharkState.hpp"


namespace kmint {
	namespace pigisland {
		class SharkWanderState : public SharkState
		{
		public:
			void Execute(shark* shark) override;
		};
	}
}


