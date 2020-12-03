#include "kmint/pigisland/states/piggybanking_state.hpp"
#include "kmint/random.hpp"

namespace kmint {
    namespace pigisland {
        void piggybanking_state::enter(boat* boat) {
            std::cout << "boat entering piggybanking" << std::endl;
        }
        void piggybanking_state::execute(boat* boat) {
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