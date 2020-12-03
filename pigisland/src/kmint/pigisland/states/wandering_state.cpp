#include "kmint/pigisland/states/wandering_state.hpp"
#include "kmint/random.hpp"

namespace kmint {
    namespace pigisland {
        void wandering_state::enter(shark* shark) {
            std::cout << "entering wandering" << std::endl;
        }
        void wandering_state::execute(shark* shark) {
            int next_index = random_int(0, shark->node().num_edges());
            shark->node(shark->node()[next_index].to());
        }
        void wandering_state::exit(shark* shark) {
            std::cout << "leaving wandering" << std::endl;
        }
    }
}