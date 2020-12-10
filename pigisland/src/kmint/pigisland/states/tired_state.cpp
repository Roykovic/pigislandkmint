#include "kmint/pigisland/states/tired_state.hpp"
#include "kmint/pigisland/states/wandering_state.hpp"
#include "kmint/random.hpp"
#include <kmint\pigisland\node_algorithm.hpp>
#include <kmint\pigisland\astar.hpp>

namespace kmint {
    namespace pigisland {
        void tired_state::enter(shark* shark) {
            std::cout << "entering tired" << std::endl;
            shark->set_tint(0, 0, 255);

            auto& g = *shark->graph;
            auto& sharkNode = find_node_of_kind(g, 'K');

            astar astar;
            auto path = astar.findPath(shark->node().node_id(), sharkNode.node_id(), *shark->graph, true);
            shark->set_path(path);

        }
        void tired_state::execute(shark* shark) {

            auto path = shark->get_path();
            auto& g = *shark->graph;
            if (path.size() > 0) {
                shark->node(g[path.at(0)]);
                path.erase(path.begin());
                shark->set_path(path);
            }
            else {
                wandering_state* newState = new wandering_state();
                shark->changeState(newState);
            }

        }
        void tired_state::exit(shark* shark) {
            std::cout << "leaving tired" << std::endl;
            auto& g = *shark->graph;
            g.untag_all();
            shark->remove_tint();
            shark->set_steps(0);
            shark->message_->send();
        }
    }
}