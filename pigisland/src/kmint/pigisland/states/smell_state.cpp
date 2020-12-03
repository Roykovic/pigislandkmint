#include "kmint/pigisland/states/smell_state.hpp"
#include "kmint/pigisland/states/hunting_state.hpp"
#include "kmint/pigisland/states/wandering_state.hpp"
#include "kmint/random.hpp"

namespace kmint {
    namespace pigisland {
        void smell_state::enter(shark* shark) {
            std::cout << "entering smelling" << std::endl;
        }
        void smell_state::execute(shark* shark) {
            for (auto i = shark->begin_perceived(); i != shark->end_perceived(); ++i) {
                auto const& a = *i;
                std::cout << "Smelled a pig at " << a.location().x() << ", "
                    << a.location().y() << "\n";
                shark->set_pigloc(a.location());

                hunting_state* newState = new hunting_state();
                shark->changeState(newState);
                return;
            }
            wandering_state* newState = new wandering_state();
            shark->changeState(newState);
        }
        void smell_state::exit(shark* shark) {
            std::cout << "leaving smelling" << std::endl;
        }
    }
}