#include "kmint/pigisland/shark.hpp"
#include "kmint/pigisland/node_algorithm.hpp"
#include "kmint/pigisland/resources.hpp"
#include "kmint/random.hpp"
#include "kmint/pigisland/Aster.hpp"
#include "kmint/pigisland/pig.hpp"
#include "kmint/pigisland/States/SharkStates/SharkChaseState.hpp"
#include "kmint/pigisland/States/SharkStates/SharkWanderState.hpp"

namespace kmint::pigisland
{
  shark::shark(map::map_graph& g, map::map_node& initial_node)
    : play::map_bound_actor{ initial_node },
	  drawable_{ *this, graphics::image{shark_image()} }, graph_{ g }, state_{new SharkWanderState(this)} {}

  void shark::setState(SharkState* state)
  {
	  delete state_;
	  state_ = state;
      
  }

  void shark::act(delta_time dt) {
    t_passed_ += dt;
    if (to_seconds(t_passed_) >= 1) {

      t_passed_ = from_seconds(0);
    	
	  state_->Execute(this);
	  
	  if (node().node_info().kind == 'R') {
		  t_passed_ = from_seconds(-1 * 4);
	  }
	  else {
		  t_passed_ = from_seconds(0);
	  }
    }
  }
} // namespace kmint
