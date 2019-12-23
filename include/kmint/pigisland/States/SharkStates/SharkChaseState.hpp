#pragma once
#include "SharkState.hpp"

		class SharkChaseState : public SharkState
		{
		public:
			SharkChaseState(kmint::pigisland::shark* shark);
			void Execute(kmint::pigisland::shark* shark) override;
		};


