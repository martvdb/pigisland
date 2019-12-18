#ifndef KMINT_PIGISLAND_DOCK_HPP
#define KMINT_PIGISLAND_DOCK_HPP
#pragma once
#include "kmint/map/map.hpp"
#include "kmint/play.hpp"
#include "kmint/primitives.hpp"
#include "kmint/pigisland/boat.hpp"

namespace kmint {
	namespace pigisland {
		class boat;
		class dock {
		public:
			dock(int minRepair, int maxRepair, int initial_node);
			void repair(boat* boat) const;
			int dockNode() { return _dockNode; }
		private:
			int _minRepair;
			int _maxRepair;
			int _dockNode;
		};

	}
}

#endif /* KMINT_PIGISLAND_DOCK_HPP */
