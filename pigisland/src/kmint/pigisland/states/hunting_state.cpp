#include "kmint/pigisland/states/hunting_state.hpp"
#include "kmint/pigisland/states/wandering_state.hpp"
#include "kmint/random.hpp"
#include <kmint\pigisland\astar.hpp>
#include <kmint\pigisland\node_algorithm.hpp>

namespace kmint {
    namespace pigisland {
        void hunting_state::enter(shark* shark) {
            std::cout << "entering hunting" << std::endl;
            shark->set_tint(255, 0, 0);

            auto& closestNode = getClosestNode(shark->get_pigloc(), shark->graph);
            auto& g = *shark->graph;

            if (closestNode.node_id() == find_node_of_kind(g, 'K').node_id()) {
                wandering_state* newState = new wandering_state();
                shark->changeState(newState);
                return;
            }

            auto path = shark->astar->findPath(shark->node().node_id(), closestNode.node_id(), *shark->graph, true, "shark");
            shark->set_path(path);
        }
        void hunting_state::execute(shark* shark) {
            auto path = shark->get_path();
            auto& g = *shark->graph;
            if (path.size() > 0) {
                shark->node(g[path.at(0)]);
                path.erase(path.begin());
                shark->set_path(path);
            }
        }
        void hunting_state::exit(shark* shark) {
            std::cout << "leaving hunting" << std::endl;
            auto& g = *shark->graph;
            //g.untag_all();

            for (auto const& [index, visited] : shark->astar->visitedNodesByActor["shark"]) {
                if (visited) {
                    g[index].tag(kmint::graph::node_tag::normal);
                }
            }

            shark->astar->visitedNodesByActor["shark"].clear();

            shark->remove_tint();
        }


        kmint::map::map_node& hunting_state::getClosestNode(kmint::math::vector2d loc, kmint::map::map_graph* graph) {


            auto& g = *graph;
            int nodeXIndex = (((loc.x() - 16)/ 32) + 0.5);
            int nodeX = (nodeXIndex * 32) + 16;

            int nodeYIndex = (((loc.y() - 16) / 32) + 0.5);
            int nodeY = (nodeYIndex * 32) + 16;

            auto nodeLoc = kmint::math::vector2d(nodeX, nodeY);

            std::cout << "Knoop op: " << nodeX << ", "
                << nodeY << "\n";


            for (std::size_t i = 0; i < g.num_nodes(); ++i) {
                if (g[i].location() == nodeLoc) {
                    return g[i];
                }
            }
           
            return find_node_of_kind(g, 'K');
        }
    }
}