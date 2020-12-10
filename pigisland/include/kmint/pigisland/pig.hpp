#pragma once
#ifndef KMINT_PIGISLAND_PIG_HPP
#define KMINT_PIGISLAND_PIG_HPP

#include "kmint/play.hpp"
#include "kmint/play/image_drawable.hpp"
#include <tuple>
#include <vector>
#include <kmint\pigisland\steering_behaviours.hpp>
#include <kmint/math/vector2d.hpp>
#include <kmint\pigisland\shark.hpp>
#include <kmint\pigisland\boat.hpp>
#include "kmint/random.hpp"

namespace kmint {
namespace pigisland {

class pig : public play::free_roaming_actor {
public:
  pig(math::vector2d location, boat& boat, shark& shark);
  const ui::drawable &drawable() const override { return drawable_; }
  void act(delta_time dt) override;
  void move(math::vector2d delta);
  double get_max_speed() { return maxSpeed_; }
  kmint::math::vector2d get_velocity() { return velocity_; }
  math::vector2d heading() const override { return heading_; }

  bool incorporeal() const override { return false; }

  scalar collision_range() const override { return 16.0; }
private:
  play::image_drawable drawable_;

  steering_behaviours* steering;

  scalar sharkAttraction = random_scalar(-1, 1);
  scalar boatAttraction = random_scalar(-1, 1);
  scalar cohesion = random_scalar(0, 1);
  scalar seperation = random_scalar(0, 1);
  scalar alignment = random_scalar(0, 1);

  double mass_ = 1;
  double maxSpeed_ = 100;
  kmint::math::vector2d heading_ = kmint::math::vector2d{ 1,0 };
  kmint::math::vector2d velocity_ = kmint::math::vector2d{ 0,0 };


  // hoeveel tijd is verstreken sinds de laatste beweging
  delta_time t_passed_{};


  boat& boat_;
  shark& shark_;
};

} // namespace pigisland
} // namespace kmint

#endif /* KMINT_PIGISLAND_PIG_HPP */
