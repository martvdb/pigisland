#include "kmint/pigisland/boat.hpp"
#include "kmint/pigisland/States/BoatStates/BoatWanderState.hpp"


namespace kmint {
namespace pigisland {
  boat::boat(map::map_graph& g, map::map_node& initial_node)
    : play::map_bound_actor{ initial_node },
	  drawable_{ *this, graphics::image{boat_image()}}, graph_{ g } 
  {
	  docks_.push_back(dock(30, 50, find_node_of_kind(g, '1').node_id()));
	  docks_.push_back(dock(20, 100, find_node_of_kind(g, '2').node_id()));
	  docks_.push_back(dock(50, 50, find_node_of_kind(g, '3').node_id()));
	  state_ = new BoatWanderState();
  }

  void boat::setState(BoatState* state)
  {
	  delete state_;
	  state_ = state;
  }

  void boat::act(delta_time dt) {
    t_passed_ += dt;
    if (to_seconds(t_passed_) >= 1) 
	{
		state_->Execute(this);
		if (node().node_info().kind == 'R') {
			t_passed_ = from_seconds(-1 * 4);
		}
		else {
			t_passed_ = from_seconds(0);
		}
		
		
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
