#include "..\..\..\include\kmint\pigisland\Aster.hpp"
#include <deque>
#include <map>
#include <algorithm>
#include <kmint\pigisland\resources.hpp>

int calculateRoute(size_t start_id , size_t target_id, kmint::map::map_graph &g) {
	auto& graph = g;
	auto& target = graph[target_id];
	auto& start = graph[start_id];
	std::deque<int> queue;
	std::map<size_t, int> distances;
	std::deque<size_t> visited;
	std::map<size_t, int> costs;
	std::map<size_t, size_t> predecessors;

	for (std::size_t i = 0; i < graph.num_nodes(); ++i) {
		costs.emplace(graph[i].node_id(), INT_MAX);
	}

	costs[start_id] = 0;
	queue.push_back(0);
	distances.emplace(start_id, 0);
	graph.untag_all();

	while (queue.size() > 0) {

		int key = 0;
		for (auto& i : distances) {
			if (i.second == queue.front()) {
				key = i.first;
				break; // to stop searching
			}
		}

		auto nodeid = key;
		queue.pop_front();
		visited.push_front(nodeid);
		auto& node = graph[nodeid];

		for (std::size_t i = 0; i < node.num_edges(); ++i) {
			//calculate distance
			auto& to = node[i].to();
			kmint::math::basic_vector2d<float> targetLocation = target.location();
			kmint::math::basic_vector2d<float> toLocation = to.location();
			kmint::math::basic_vector2d<float> nodeLocation = node.location();

			int distance = calculateDistance(targetLocation.x(), toLocation.x()) + calculateDistance(targetLocation.y(), toLocation.y());
			int nodeDistance = calculateDistance(targetLocation.x(), nodeLocation.x()) + calculateDistance(targetLocation.y(), nodeLocation.y());

			if (std::find(visited.begin(), visited.end(), to.node_id()) == visited.end()) {

				queue.push_back(distance);
				distances.emplace(to.node_id(), distance);
			}
			graph[to.node_id()].tag(kmint::graph::node_tag::visited);
			if (costs[to.node_id()] > costs[nodeid] + node[i].weight()) {
				costs[to.node_id()] = costs[nodeid] + node[i].weight();
				predecessors.emplace(to.node_id(), nodeid);
				if (to.node_id() == target_id) {
					queue.clear();
					break;
				}
			}

		}
		std::sort(queue.begin(), queue.end());

	}
	int current = target_id;
	while (true) {
		if (current == start_id) {
			break;
		}
		graph[current].tag(kmint::graph::node_tag::path);
		if (predecessors[current] == start_id) {
			for (std::size_t i = 0; i < start.num_edges(); ++i) {
				if (start[i].to().node_id() == current) {
					return i;
				}
			}
		}
		current = predecessors[current];
	}
	return 0;
}

int calculateDistance(int x1, int x2) {

	if (x1 - x2 < 0) {
		return (x1 - x2) * -1;
	}
	return x1 - x2;
}

