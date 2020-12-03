#include "kmint/pigisland/astar.hpp"

namespace kmint {
namespace pigisland {


	astar::astar()
	{
	}
	astar::~astar()
	{
	}

	std::vector<size_t> astar::findPath(size_t start, size_t end, kmint::map::map_graph& graph, bool checkWeight) {
		std::map<size_t, int> distanceMap;
		std::map<size_t, size_t> pathMap;
		std::map<size_t, bool> visited;

		for (size_t i = 0; i < graph.num_nodes(); ++i) {
			distanceMap[i] = -1;
			visited[i] = false;;
		}
		distanceMap[start] = 0;

		std::map<size_t, size_t> astarMap(astarRecusive(start, end, visited, distanceMap, pathMap, graph, checkWeight));
		return shortestPath(start, end, astarMap, graph);
	}

	std::map<size_t, size_t> astar::astarRecusive(size_t current, size_t end, std::map<size_t, bool> visited, std::map<size_t, int> distanceMap, std::map<size_t, size_t> pathMap, kmint::map::map_graph& graph, bool checkWeight)
	{
		visited[current] = true;
		graph[current].tag(kmint::graph::node_tag::visited);

		if (current == end) {
			return pathMap;
		}

		for (std::size_t i = 0; i < graph[current].num_edges(); ++i) {

			if (visited[graph[current][i].to().node_id()] == false) {	//if neighbour is not visited
				//int currentDistance = distanceMap[current];
				int distance = distanceMap[current] + calculateHeuristicWeight(graph[current][i].to().node_id(), end, graph);
//				int distance = distanceMap[current];
				if (checkWeight) {
					distance += graph[current][i].weight();
				}
				else {
					distance += 1;
				}

				if (distanceMap[graph[current][i].to().node_id()] > distance || distanceMap[graph[current][i].to().node_id()] == -1) {
					distanceMap[graph[current][i].to().node_id()] = distance;
					pathMap[graph[current][i].to().node_id()] = current;
				}
			}
		}

		size_t next = findShortestDistanceNode(visited, distanceMap);
		return astarRecusive(next, end, visited, distanceMap, pathMap, graph, checkWeight);
	}

	size_t astar::findShortestDistanceNode(std::map<size_t, bool> visited, std::map<size_t, int> distanceMap) {
		size_t shortestIndex = -1;

		for (std::map<size_t, bool>::iterator it = visited.begin(); it != visited.end(); ++it)
		{
			if (it->second == false) {
				if (distanceMap[it->first] != -1) {
					if (shortestIndex == -1) {
						shortestIndex = it->first;
					}
					else if(distanceMap[it->first] < distanceMap[shortestIndex]){
						shortestIndex = it->first;
					}
				}
			}
		}
		return shortestIndex;
	}

	std::vector<size_t> astar::shortestPath(size_t start, size_t end, std::map<size_t, size_t> visited, kmint::map::map_graph& graph) {

		std::vector<size_t> path;

		while (start != end) {
			path.push_back(end);
			graph[end].tag(kmint::graph::node_tag::path);
			end = visited[end];
		}

		path.push_back(start);
		graph[start].tag(kmint::graph::node_tag::path);
		std::reverse(path.begin(), path.end());
		return path;
	}

	int astar::calculateHeuristicWeight(size_t start, size_t end, kmint::map::map_graph& graph) {

		//als heuristic weight wordt hier de absolute afstand tot de locatie van de haas genomen
		int dx = abs(graph[start].location().x() - graph[end].location().x());
		int dy = abs(graph[start].location().y() - graph[end].location().y());
		return dx+dy;
	}

} // namespace pigisland
} // namespace kmint