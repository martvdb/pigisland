#include "kmint\pigisland\dock.hpp"
#include "../../../../libkmint/src/kmint/random.cpp"
namespace kmint {
namespace pigisland {

dock::dock(int minRepair, int maxRepair, int initial_node) : _minRepair{ minRepair }, _maxRepair{ maxRepair }, _dockNode{ initial_node } {}

void dock::repair(boat* boat) const
{
	boat->repair(random_int(_minRepair, _maxRepair));
}

}
}