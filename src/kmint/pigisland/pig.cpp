#include "kmint/pigisland/pig.hpp"
#include "kmint/pigisland/resources.hpp"
#include "kmint/random.hpp"

namespace kmint {
namespace pigisland {


pig::pig(math::vector2d location)
  : play::free_roaming_actor{location},
    drawable_{*this, pig_image()} {
}

void pig::act(delta_time dt) {


	for (auto i = begin_perceived(); i != end_perceived(); ++i) {
		int o = 0;
		//const map::map_node& node = find_closest_node_to(graph_, i->location());
		//int node_id = node.node_id();
		//next_index = calculateRoute(this->node().node_id(), node_id, graph_);
	}
}


} // namespace pigisland

} // namespace kmint
