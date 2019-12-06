#include "kmint/pigisland/shark.hpp"
#include "kmint/pigisland/node_algorithm.hpp"
#include "kmint/pigisland/resources.hpp"
#include "kmint/random.hpp"
namespace kmint {
namespace pigisland {
  shark::shark(map::map_graph& g, map::map_node& initial_node)
    : play::map_bound_actor{ initial_node },
      drawable_{ *this, graphics::image{shark_image()} } {}


  void shark::act(delta_time dt) {
    t_passed_ += dt;
    if (to_seconds(t_passed_) >= 1) {
      // pick random edge
      int next_index = random_int(0, node().num_edges());
      this->node(node()[next_index].to());
      t_passed_ = from_seconds(0);
    }
  }

} // namespace pigisland
} // namespace kmint
