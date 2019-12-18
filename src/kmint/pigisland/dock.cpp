#include "kmint\pigisland\dock.hpp"
#include "../../../../libkmint/src/kmint/random.cpp"
namespace kmint {
namespace pigisland {

dock::dock(int minRepair, int maxRepair, int initial_node) : _minRepair{ minRepair }, _maxRepair{ maxRepair }, _dockNode{ initial_node } {}

void dock::repair(boat& boat) const
{
	if (_minRepair == _maxRepair) {
		boat.repair(_maxRepair);
	}
	else {
		boat.repair(random_int(_minRepair, _maxRepair));
	}
	
}

}
}