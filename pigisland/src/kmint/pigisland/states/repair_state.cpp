#include "kmint/pigisland/states/repair_state.hpp"
#include "kmint/pigisland/states/wandering_boat_state.hpp"
#include "kmint/random.hpp"
#include <kmint\pigisland\node_algorithm.hpp>
#include <kmint\pigisland\astar.hpp>
#include <numeric>

namespace kmint {
    namespace pigisland {
        void repair_state::enter(boat* boat) {
            std::cout << "boat entering repair state" << std::endl;

            boat->set_tint(255, 165, 0);

            auto& g = *boat->graph;
            auto& grainNode = find_node_of_kind(g, '1');
            auto& grassNode = find_node_of_kind(g, '2');
            auto& treeNode  = find_node_of_kind(g, '3');

            const float random_number = random_scalar(0, 1);
            std::cout << random_number << std::endl;
            astar astar;

            auto grainPath = astar.findPath(boat->node().node_id(), grainNode.node_id(), *boat->graph, true);
            auto grassPath = astar.findPath(boat->node().node_id(), grassNode.node_id(), *boat->graph, true);
            auto treePath = astar.findPath(boat->node().node_id(), treeNode.node_id(), *boat->graph, true);

            auto grainChance = (std::accumulate(boat->repair_history_[1].begin(), boat->repair_history_[1].end(), 0.0) / boat->repair_history_[1].size()*15) / (grainPath.size());
            auto grassChance = (std::accumulate(boat->repair_history_[2].begin(), boat->repair_history_[2].end(), 0.0) / boat->repair_history_[2].size()*15) / (grassPath.size());
            auto treeChance  = (std::accumulate(boat->repair_history_[3].begin(), boat->repair_history_[3].end(), 0.0) / boat->repair_history_[3].size()*15) / (treePath.size());
                                                                                                                                      
            grainEfficiency = grainChance / (grainChance + grassChance + treeChance);
            grassEfficiency = grassChance / (grainChance + grassChance + treeChance);
            treeEfficiency  = treeChance / (grainChance + grassChance + treeChance);

            std::vector<size_t> path;

            if (random_number < grainEfficiency) {
                std::cout << "Path A" << std::endl;
                dock = 1;
                path = grainPath;
            }
            else if(random_number < grainEfficiency + grassEfficiency){
                std::cout << "Path B" << std::endl;
                dock = 2;
                path = grassPath;
            }
            else {
                std::cout << "Path C" << std::endl;
                dock = 3;
                path = treePath;
            }

            boat->set_path(path);

        }
        void repair_state::execute(boat* boat) {
            auto path = boat->get_path();
            auto& g = *boat->graph;
            if (path.size() > 0) {
                boat->node(g[path.at(0)]);
                path.erase(path.begin());
                boat->set_path(path);
            }
            else {
                boat->changeState(new wandering_boat_state());
            }
        }
        void repair_state::exit(boat* boat) {
            std::cout << "boat leaving repair state" << std::endl;
            auto& g = *boat->graph;
            g.untag_all();
            boat->remove_tint();

            int repair = 0;

            switch (dock) {
            case 1:
                repair = random_int(30, 50);
                break;
            case 2:
                repair = random_int(20, 100);
                break;
            case 3:
                repair = 50;
                break;
            default:
                break;
            }

            boat->repair_history_[dock].push_back(repair);

            boat->set_damage(boat->get_damage() - repair); //TODO randomized damage repair
        }
    }
}