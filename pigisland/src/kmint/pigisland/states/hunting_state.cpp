#include "kmint/pigisland/states/hunting_state.hpp"
#include "kmint/random.hpp"
#include <kmint\pigisland\astar.hpp>

namespace kmint {
    namespace pigisland {
        void hunting_state::enter(shark* shark) {
            std::cout << "entering hunting" << std::endl;
            auto& closestNode = getClosestNode(shark->get_pigloc(), shark->graph);

            astar astar;
            auto path = astar.findPath(shark->node().node_id(), closestNode.node_id(), *shark->graph, true);
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
        }


        kmint::map::map_node& hunting_state::getClosestNode(kmint::math::vector2d loc, kmint::map::map_graph* graph) {

            auto& g = *graph;
            int nodeXIndex = ((loc.x() / 32) + 0.5);
            int nodeX = (nodeXIndex * 32) + 16;

            int nodeYIndex = ((loc.y() / 32) + 0.5);
            int nodeY = (nodeYIndex * 32) + 16;

            auto nodeLoc = kmint::math::vector2d(nodeX, nodeY);

            std::cout << "Knoop op: " << nodeX << ", "
                << nodeY << "\n";


            for (std::size_t i = 0; i < g.num_nodes(); ++i) {
                if (g[i].location() == nodeLoc) {
                    return g[i];
                }
            }
        }
    }
}