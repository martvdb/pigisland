#ifndef KMINT_PIGISLAND_PIG_HPP
#define KMINT_PIGISLAND_PIG_HPP

#include "kmint/play.hpp"
#include "kmint/pigisland/SteeringBehaviors.hpp"
#include <tuple>
#include <vector>

namespace kmint {
namespace pigisland {
class pig : public play::free_roaming_actor {
public:
  pig(math::vector2d location);
  const ui::drawable &drawable() const override { return drawable_; }
  bool perceptive() const override { return true; }
  scalar perception_range() const override { return 150.0f; }
  bool perceivable() const override { return true; }
  bool incorporeal() const override { return false; }
  std::string type() const override { return "pig"; }
  void act(delta_time dt) override;
  SteeringBehaviors Steering;
private:
  play::image_drawable drawable_;
};

} // namespace pigisland
} // namespace kmint

#endif /* KMINT_PIGISLAND_PIG_HPP */
