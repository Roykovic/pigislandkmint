#include "kmint/pigisland/states/piggybanking_state.hpp"
#include "kmint/pigisland/states/repair_state.hpp"
#include "kmint/random.hpp"

namespace kmint {
    namespace pigisland {
        void piggybanking_state::enter(boat* boat) {
            std::cout << "boat entering piggybanking" << std::endl;
        }
        void piggybanking_state::execute(boat* boat) {

            if (boat->get_damage() > 99) {
                boat->changeState(new repair_state());
            }
            else if(to_seconds(boat->get_t_passed()) > 1) {
                boat->set_damage(boat->get_damage() + 1);
            }

            for (auto j = boat->begin_collision(); j != boat->end_collision(); ++j) {
                auto& c = *j;

                if (!strstr(typeid(c).name(), "boat")) {
                    std::cout << "picked up pig at " << c.location().x() << ", "
                        << c.location().y() << "\n";

                    c.remove();
                }
            }
        }
        void piggybanking_state::exit(boat* boat) {
            std::cout << "boat leaving piggybanking" << std::endl;
        }
    }
}