#include "kmint/pigisland/shark.hpp"
#include "kmint/pigisland/node_algorithm.hpp"
#include "kmint/pigisland/resources.hpp"
#include "kmint/random.hpp"
#include "kmint/pigisland/Aster.hpp"
namespace kmint {
namespace pigisland {
  shark::shark(map::map_graph& g, map::map_node& initial_node)
    : play::map_bound_actor{ initial_node },
	  drawable_{ *this, graphics::image{shark_image()}}, graph_{ g } {}


  void shark::act(delta_time dt) {
    t_passed_ += dt;
    if (to_seconds(t_passed_) >= 1) {
      // pick random edge
      int next_index = random_int(0, node().num_edges());

      t_passed_ = from_seconds(0);

	  for (auto i = begin_perceived(); i != end_perceived(); ++i) {
		 const map::map_node &node = find_closest_node_to(graph_, i->location());
		 int node_id = node.node_id();
		 next_index = calculateRoute(this->node().node_id(), node_id, graph_);
	  }

	  if (amountOfSteps_ >= 100) {
		  next_index = calculateRoute(this->node().node_id(), find_shark_resting_place(graph_).node_id(), graph_);
	  }

	  this->node(node()[next_index].to());
	  amountOfSteps_++;
    }
  }

} // namespace pigisland
} // namespace kmint
