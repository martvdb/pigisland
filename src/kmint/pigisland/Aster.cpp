#include "..\..\..\include\kmint\pigisland\Aster.hpp"
#include <deque>
#include <map>
#include <algorithm>
#include <kmint\pigisland\resources.hpp>
bool sortbysec(const std::pair<int, int>& a,
	const std::pair<int, int>& b)
{
	return (a.second < b.second);
}

int calculateRoute(size_t start_id , size_t target_id, kmint::map::map_graph &g) {
	auto& graph = g;
	auto& target = graph[target_id];
	auto& start = graph[start_id];
	std::deque<std::pair<int, int>> queue;
	std::deque<size_t> visited;
	std::map<size_t, int> costs;
	std::map<size_t, size_t> predecessors;

	for (std::size_t i = 0; i < graph.num_nodes(); ++i) {
		costs.emplace(graph[i].node_id(), INT_MAX);
	}

	costs[start_id] = 0;
	queue.emplace_back(std::make_pair(start_id, 0));
	graph.untag_all();

	while (!queue.empty()) {
		auto nodeid = queue.front().first;
		queue.pop_front();
		visited.push_front(nodeid);
		auto& node = graph[nodeid];

		for (std::size_t i = 0; i < node.num_edges(); ++i) {
			//calculate distance
			auto& to = node[i].to();
			kmint::math::basic_vector2d<float> targetLocation = target.location();
			kmint::math::basic_vector2d<float> nodeLocation = node.location();
			int nodeDistance = calculateDistance(targetLocation.x(), nodeLocation.x()) + calculateDistance(targetLocation.y(), nodeLocation.y());
			kmint::math::basic_vector2d<float> toLocation = to.location();
			

			int distance = calculateDistance(targetLocation.x(), toLocation.x()) + calculateDistance(targetLocation.y(), toLocation.y());
			
			std::vector<int> nodes;
			std::transform(queue.begin(), queue.end(),
				std::back_inserter(nodes),
				[](auto const& pair) { return pair.first; });
			if (distance < nodeDistance && (std::find(visited.begin(), visited.end(), to.node_id()) == visited.end() || costs[to.node_id()] > costs[nodeid] + node[i].weight())) {
				
				if(std::find(nodes.begin(), nodes.end(), to.node_id()) == nodes.end())
				{
					queue.emplace_back(std::make_pair(to.node_id(), distance));
				}
				
			}
			graph[node.node_id()].tag(kmint::graph::node_tag::visited);
			if (costs[to.node_id()] >= costs[nodeid] + node[i].weight()) {
				costs[to.node_id()] = costs[nodeid] + node[i].weight();
				predecessors[to.node_id()] = nodeid;
			}

		}
		sort(queue.begin(), queue.end(), sortbysec);

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

