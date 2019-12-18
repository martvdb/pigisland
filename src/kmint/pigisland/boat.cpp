#include "kmint/pigisland/boat.hpp"


namespace kmint {
namespace pigisland {
  boat::boat(map::map_graph& g, map::map_node& initial_node)
    : play::map_bound_actor{ initial_node },
	  drawable_{ *this, graphics::image{boat_image()}}, graph_{ g } 
  {
	  docks_.push_back(dock(30, 50, find_node_of_kind(g, '1').node_id()));
	  docks_.push_back(dock(20, 100, find_node_of_kind(g, '2').node_id()));
	  docks_.push_back(dock(50, 50, find_node_of_kind(g, '3').node_id()));
  }


  void boat::act(delta_time dt) {
    t_passed_ += dt;
    if (to_seconds(t_passed_) >= 1) 
	{
		int next_index;
		if (paintDamage_ < 100) {
			// pick random edge
			for (auto i = begin_collision(); i != end_collision(); ++i) {
				if (i->type() == "pig") {
					i->remove();
				}
			}
			next_index = random_int(0, node().num_edges());
			paintDamage_++;
			if (paintDamage_ >= 100) {
				mooringPlace = find_random_mooring_place(graph_).node_id();
			}
		}
		else {
			if (this->node().node_id() == mooringPlace) {
				auto i = std::find_if(docks_.begin(), docks_.end(), [this](auto& dock) {
					return dock.dockNode() == mooringPlace;
				});
				i->repair(*this);
				next_index = random_int(0, node().num_edges());
			}
			else {
				next_index = calculateRoute(this->node().node_id(), mooringPlace, graph_);
			}
		}
		if (node()[next_index].weight() > 1) {
			t_passed_ = from_seconds(-1 * node()[next_index].weight());
		}
		else {
			t_passed_ = from_seconds(0);
		}
		this->node(node()[next_index].to());
		
    }
  }
  void boat::repair(int amountOfRepairPoints) {
	  paintDamage_ -= amountOfRepairPoints;
	  if (paintDamage_ < 0) {
		  paintDamage_ = 0;
	  }
	  
  }

} // namespace pigisland
} // namespace kmint
