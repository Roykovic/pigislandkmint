#include "kmint/pigisland/states/wandering_boat_state.hpp"
#include "kmint/pigisland/states/repair_state.hpp"
#include "kmint/random.hpp"

namespace kmint {
    namespace pigisland {
        void wandering_boat_state::enter(boat* boat) {
            std::cout << "boat entering wandering" << std::endl;
        }
        void wandering_boat_state::execute(boat* boat) {

            if (boat->get_damage() > 99) {
                boat->changeState(new repair_state());
                return;
            }

            int next_index = random_int(0, boat->node().num_edges());
            boat->node(boat->node()[next_index].to());
            boat->set_damage(boat->get_damage() + 1);
        }
        void wandering_boat_state::exit(boat* boat) {
            std::cout << "boat leaving wandering" << std::endl;
        }
    }
}