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
      drawable_{ *this, graphics::image{boat_image()} }, currentstate_(new wandering_boat_state()), globalstate_(new piggybanking_state()), graph(&g) {}


  void boat::act(delta_time dt) {
    t_passed_ += dt;
    globalstate_->execute(this);
    if (to_seconds(t_passed_) >= (this->node().node_info().kind == 'R' ? 4 : 1)) {

        currentstate_->execute(this);

      t_passed_ = from_seconds(0);
    }
  }

  void boat::changeState(State<boat>* pNewState)
  {
      if (pNewState == nullptr) { return; }

      if (currentstate_ == nullptr) {
          currentstate_ = pNewState;

          currentstate_->enter(this);
          return;
      }

      if (typeid(*pNewState) != typeid(*currentstate_)) {
          currentstate_->exit(this);
          delete currentstate_;

          currentstate_ = pNewState;

          currentstate_->enter(this);
          return;
      }
  }

  void boat::set_tint(int r, int g, int b) {
      graphics::color tint = graphics::color(r, g, b);
      this->drawable_.set_tint(tint);
  }

  void boat::remove_tint() {
      this->drawable_.remove_tint();
  }

} // namespace pigisland
} // namespace kmint
