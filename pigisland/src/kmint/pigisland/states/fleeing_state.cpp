#include "kmint/pigisland/states/fleeing_state.hpp"
#include "kmint/pigisland/states/wandering_state.hpp"
#include "kmint/random.hpp"

namespace kmint {
    namespace pigisland {
        void fleeing_state::enter(shark* shark) {
            std::cout << "entering fleeing" << std::endl;
            shark->set_tint(0,255,0);
            shark->set_fleeing(true);
        }
        void fleeing_state::execute(shark* shark) {
            if (steps_ < 10) {
                int next_index = random_int(0, shark->node().num_edges());
                shark->node(shark->node()[next_index].to());
                ++steps_;
            }
            else {
                wandering_state* newState = new wandering_state();
                shark->changeState(newState);
            }
        }
        void fleeing_state::exit(shark* shark) {
            std::cout << "leaving fleeing" << std::endl;
            steps_ = 0;
            shark->remove_tint();
            shark->set_fleeing(false);
        }
    }
}