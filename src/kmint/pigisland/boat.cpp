#include "kmint/pigisland/boat.hpp"
#include "kmint/pigisland/node_algorithm.hpp"
#include "kmint/pigisland/resources.hpp"
#include "kmint/random.hpp"
#include "kmint/pigisland/Aster.hpp"
namespace kmint {
namespace pigisland {
  boat::boat(map::map_graph& g, map::map_node& initial_node)
    : play::map_bound_actor{ initial_node },
	  drawable_{ *this, graphics::image{boat_image()}}, graph_{ g } {}


  void boat::act(delta_time dt) {
    t_passed_ += dt;
    if (to_seconds(t_passed_) >= 1) 
	{
		int next_index;
		if (paintDamage_ < 3) {
			// pick random edge
			next_index = random_int(0, node().num_edges());
			this->node(node()[next_index].to());
			paintDamage_++;
			if (paintDamage_ == 3) {
				mooringPlace = find_random_mooring_place(graph_).node_id();
			}
		}
		else {
			next_index = calculateRoute(this->node().node_id(),mooringPlace, graph_);
			this->node(node()[next_index].to());
		}
		t_passed_ = from_seconds(0);
    }
  }

} // namespace pigisland
} // namespace kmint
