#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <tuple>
#include <set>
#include <algorithm>
#include <numeric>

using namespace std;

vector<vector<int>> Read(int& edges_num) {
	ifstream input;
	input.open("input.txt");
	vector<vector<int>> graph;

	if (input.is_open()) {
		int V, E; 
		input >> V >> E;
		graph.resize(V, vector<int>());

		for (int e = 0; e < E; ++e) {
			int from, to; 
			input >> from >> to;
			--from; --to;
			graph[from].push_back(to);
			graph[to].push_back(from);
		}

		edges_num = E;
		input.close();
	}
	return graph;
}

void Write(int res) {
	ofstream output("output.txt");
	if (output.is_open()) {
		output << res;
		output.close();
	}
}

// DFS on finding bridges in a graph
void Solve(const vector<vector<int>>& graph, int curr, int parent, int depth,
	vector<bool>& visited, vector<int>& min_depth_can_reach,
	vector<int>& entry_depth, int& res) {
	visited[curr] = true;
	min_depth_can_reach[curr] = entry_depth[curr] = depth;

	for (auto adj : graph[curr]) {
		if (adj == parent) continue;
		if (!visited[adj]) {
			Solve(graph, adj, curr, depth + 1, visited, min_depth_can_reach, entry_depth, res);
			min_depth_can_reach[curr] = min(min_depth_can_reach[curr], min_depth_can_reach[adj]);
			if (min_depth_can_reach[adj] > depth) ++res; // found a bridge
		}
		else
			min_depth_can_reach[curr] = min(min_depth_can_reach[curr], entry_depth[adj]);
	}
}

int Solve(vector<vector<int>> graph, int edges_num) {
	int N = graph.size();
	vector<bool> visited(N);
	vector<int> entry_depth(N);
	vector<int> min_depth_can_reach(N);
	int res = 0;

	for (int i = 0; i < N; ++i) {
		if (!visited[i]) 
			Solve(graph, i, -1, 0, visited, min_depth_can_reach, entry_depth, res);
	}
	
	return edges_num - res;
}

int main() {
	int edges_num = 0;
	auto graph = Read(edges_num);
	Write(Solve(graph, edges_num));
	return 0;
}

