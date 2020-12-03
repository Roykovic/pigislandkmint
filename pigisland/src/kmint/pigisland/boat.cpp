#include "kmint/pigisland/boat.hpp"
#include "kmint/pigisland/node_algorithm.hpp"
#include "kmint/pigisland/resources.hpp"
#include "kmint/random.hpp"
#include "kmint/pigisland/states/wandering_boat_state.hpp"
#include "kmint/pigisland/states/piggybanking_state.hpp"
namespace kmint {
namespace pigisland {
  boat::boat(map::map_graph& g, map::map_node& initial_node)
    : play::map_bound_actor{ initial_node },
      drawable_{ *this, graphics::image{boat_image()} }, currentstate_(new wandering_boat_state()), globalstate_(new piggybanking_state()) {}


  void boat::act(delta_time dt) {
    t_passed_ += dt;
    if (to_seconds(t_passed_) >= 1) {

        globalstate_->execute(this);
        currentstate_->execute(this);

      t_passed_ = from_seconds(0);
    }
  }

} // namespace pigisland
} // namespace kmint
