#pragma once
#include <map>
#include <vector>
#include "kmint/map/map.hpp"

namespace kmint {
	namespace pigisland {
	class astar{
	public:
		astar();
		~astar();
		std::vector<size_t> astar::findPath(size_t start, size_t end, kmint::map::map_graph& graph, bool checkWeight, std::string actor);
		std::vector<size_t> astar::shortestPath(size_t start, size_t end, std::map<size_t, size_t> visited, kmint::map::map_graph& graph);
		int calculateHeuristicWeight(size_t start, size_t end, kmint::map::map_graph& graph);
		std::map<size_t, size_t> astarRecusive(size_t current, size_t end, std::map<size_t, bool> visited, std::map<size_t, int> distanceMap, std::map<size_t, size_t> pathMap, kmint::map::map_graph& graph, bool checkWeight, std::string actor);
		size_t findShortestDistanceNode(std::map<size_t, bool> visited, std::map<size_t, int> distanceMap);
		std::map<std::string, std::map<size_t, bool>> visitedNodesByActor;
	private:
	};
	} // namespace pigisland
} // namespace kmint