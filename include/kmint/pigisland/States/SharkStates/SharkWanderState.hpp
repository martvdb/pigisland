#pragma once
#include "SharkState.hpp"

		class SharkWanderState : public SharkState
		{
		public:
			SharkWanderState(kmint::pigisland::shark* shark);
			void Execute(pigisland::shark* shark) override;
		};



