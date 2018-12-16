#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <charconv>
#include <vector>
#include <algorithm>
#include <numeric>

using namespace std;

vector<vector<int>> Read(ifstream& input) {
	vector<vector<int>> data;

	if (input.is_open()) {
		int N;
		input >> N;
		data.resize(N);

		for (int i = 0; i < N; ++i) {
			int vertex_id;
			input >> vertex_id;

			string adj_vertices_count_str;
			input >> adj_vertices_count_str;
			int adj_vertices_count = atoi(adj_vertices_count_str.c_str() + 1);
			//from_chars(&adj_vertices_count_str[1], &adj_vertices_count_str.back(), adj_vertices_count);
			data[vertex_id].resize(adj_vertices_count);

			for (int j = 0; j < adj_vertices_count; ++j)
				input >> data[vertex_id][j];
		}
	}

	return data;
}

struct Edge {
	int From;
	int To;

	Edge(int from, int to) : From(from), To(to) {}

	operator string() const noexcept {
		return to_string(From) + " - " + to_string(To);
	}
};

ostream& operator << (ostream& stream, const Edge& e) {
	stream << static_cast<string>(e);
	return stream;
};

void Write(const vector<Edge>& res) {
	cout << res.size() << "  critical links" << endl;
	for (auto e : res) cout << e << endl;
}

// DFS on finding bridges in a graph
void Solve(const vector<vector<int>>& graph, int curr, int parent, int depth,
	vector<bool>& visited, vector<int>& min_depth_can_reach,
	vector<int>& entry_depth, vector<Edge>& res) {
	visited[curr] = true;
	min_depth_can_reach[curr] = entry_depth[curr] = depth;

	for (auto adj : graph[curr]) {
		if (adj == parent) continue;
		if (!visited[adj]) {
			Solve(graph, adj, curr, depth + 1, visited, min_depth_can_reach, entry_depth, res);
			min_depth_can_reach[curr] = min(min_depth_can_reach[curr], min_depth_can_reach[adj]);
		}
		else
			min_depth_can_reach[curr] = min(min_depth_can_reach[curr], entry_depth[adj]);

		if (min_depth_can_reach[adj] > depth) // bridge found
			res.push_back(Edge(curr, adj));
	}
}

int main() {
	ifstream input;
	input.open("input.txt");

	vector<vector<int>> data;

	if (!input.is_open()) return 0;

	while (!input.eof()) {
		auto graph = Read(input);

		vector<bool> visited(graph.size());
		vector<int> entry_depth(graph.size());
		vector<int> min_depth_can_reach(graph.size());
		vector<Edge> res;
		for (int i = 0; i < visited.size(); ++i)
			if (!visited[i])
				Solve(graph, i, -1, 0, visited, min_depth_can_reach, entry_depth, res);

		Write(res);
		cout << endl;
	}

	input.close();

	int o; cin >> o;
	return 0;
}
