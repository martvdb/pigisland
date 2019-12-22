#pragma once
#include "SharkState.hpp"


namespace kmint {
	namespace pigisland {
		class SharkChaseState : public SharkState
		{
		public:
			void Execute(shark* shark) override;
		};
	}
}


