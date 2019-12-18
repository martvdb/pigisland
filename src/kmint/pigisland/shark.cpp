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
		int next_index = 0;

      t_passed_ = from_seconds(0);
	  if (amountOfSteps_ >= 100) {
		  if (this->node().node_id() == find_shark_resting_place(graph_).node_id()) {
			  amountOfSteps_ = 0;
		  }
		  else{
			  next_index = calculateRoute(this->node().node_id(), find_shark_resting_place(graph_).node_id(), graph_);
		  }
		  
	  }
	  if(amountOfSteps_ < 100) {
		  for (auto i = begin_collision(); i != end_collision(); ++i) {
			  i->remove();
		  }
		  float dist = 10000.0f;
		  kmint::math::vector2d loc = kmint::math::vector2d();
		  for (auto i = begin_perceived(); i != end_perceived(); ++i) {

			  auto const& a = *i;
			  if (distance(a.location(), this->node().location()) < dist) {
				  dist = distance(a.location(), this->node().location());
				  loc = a.location();
			  }

		  }
		  if (loc != kmint::math::vector2d(0,0)) {
			  const map::map_node& node = find_closest_node_to(graph_, loc);

			  int node_id = node.node_id();
			  if (node_id != this->node().node_id()) {
				  next_index = calculateRoute(this->node().node_id(), node_id, graph_);
			  }
			  
		  }
		  else {
			  next_index = random_int(0, node().num_edges());
		  }
	  }



	  this->node(node()[next_index].to());
	  amountOfSteps_++;
    }
  }

} // namespace pigisland
} // namespace kmint
