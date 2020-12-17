#include "kmint/pigisland/states/wandering_boat_state.hpp"
#include "kmint/pigisland/states/repair_state.hpp"
#include "kmint/random.hpp"

namespace kmint {
    namespace pigisland {
        void wandering_boat_state::enter(boat* boat) {
            std::cout << "boat entering wandering" << std::endl;
        }
        void wandering_boat_state::execute(boat* boat) {
            int next_index = random_int(0, boat->node().num_edges());
            boat->node(boat->node()[next_index].to());
        }
        void wandering_boat_state::exit(boat* boat) {
            std::cout << "boat leaving wandering" << std::endl;
        }
    }
}