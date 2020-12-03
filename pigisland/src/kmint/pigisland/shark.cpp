#include "kmint/pigisland/shark.hpp"
#include "kmint/pigisland/node_algorithm.hpp"
#include "kmint/pigisland/resources.hpp"

#include <iostream>
#include <kmint\pigisland\states\wandering_state.hpp>
#include <kmint\pigisland\states\hunting_state.hpp>
#include <kmint\pigisland\states\smell_state.hpp>
namespace kmint {
namespace pigisland {
shark::shark(map::map_graph &g, map::map_node &initial_node)
    : play::map_bound_actor{initial_node}, drawable_{*this,
                                                     graphics::image{
                                                         shark_image()}}, globalstate_(new smell_state()), currentstate_(new wandering_state), graph(&g) {}

void shark::act(delta_time dt) {
  t_passed_ += dt;
  if (to_seconds(t_passed_) >= 0.5) {

      globalstate_->execute(this);
      currentstate_->execute(this);

      t_passed_ = from_seconds(0);
  }
}

void shark::changeState(State<shark>* pNewState)
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

} // namespace pigisland
} // namespace kmint
