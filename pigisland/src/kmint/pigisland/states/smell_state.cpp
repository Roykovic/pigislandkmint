#include "kmint/pigisland/states/smell_state.hpp"
#include "kmint/pigisland/states/hunting_state.hpp"
#include "kmint/pigisland/states/wandering_state.hpp"
#include "kmint/pigisland/states/fleeing_state.hpp"
#include "kmint/pigisland/states/tired_state.hpp"
#include "kmint/random.hpp"

namespace kmint {
    namespace pigisland {
        void smell_state::enter(shark* shark) {
            std::cout << "entering smelling" << std::endl;
        }
        void smell_state::execute(shark* shark) {

            if (shark->get_steps() > 99) {
                tired_state* newState = new tired_state();
                shark->changeState(newState);
                return;
            }

            for (auto j = shark->begin_collision(); j != shark->end_collision(); ++j) {
                auto& c = *j;

                if (!strstr(typeid(c).name(), "boat")) {
                    std::cout << "Collided with pig at " << c.location().x() << ", "
                        << c.location().y() << "\n";

                    c.remove();
                    wandering_state* newState = new wandering_state();
                    shark->changeState(newState);
                }
            }

            if (to_seconds(shark->get_t_passed()) < 1) { return; }

            shark->set_steps(shark->get_steps() + 1);

            if(shark->is_fleeing()){
                return;
            }

            for (auto i = shark->begin_perceived(); i != shark->end_perceived(); ++i) {
                auto const& a = *i;

                if (strstr(typeid(a).name(), "boat")) {
                    std::cout << "Smelled Porcus Vincit at " << a.location().x() << ", "
                        << a.location().y() << "\n";
                    fleeing_state* newState = new fleeing_state();
                    shark->changeState(newState);
                }
                else {
                    if (!a.removed()) {
                        std::cout << "Smelled a pig at " << a.location().x() << ", "
                            << a.location().y() << "\n";
                        shark->set_pigloc(a.location());

                        hunting_state* newState = new hunting_state();
                        shark->changeState(newState);
                    }
                }
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