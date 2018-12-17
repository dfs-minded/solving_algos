#pragma once

#include <iostream>
#include <string>
#include <charconv>
#include <vector>
#include <algorithm>
#include <numeric>

using namespace std;

vector<vector<int>> Read(int N) {
	vector<vector<int>> data(N);

	for (int i = 0; i < N; ++i) {
		int vertex_id;
		cin >> vertex_id;

		string adj_vertices_count_str;
		cin >> adj_vertices_count_str;
		int adj_vertices_count = atoi(adj_vertices_count_str.c_str() + 1);
		//from_chars(&adj_vertices_count_str[1], &adj_vertices_count_str.back(), adj_vertices_count);
		data[vertex_id].resize(adj_vertices_count);

		for (int j = 0; j < adj_vertices_count; ++j)
			cin >> data[vertex_id][j];
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

	bool operator < (const Edge& other) const {
		return From < other.From ? true : To < other.To;
	}
};

ostream& operator << (ostream& stream, const Edge& e) {
	stream << static_cast<string>(e);
	return stream;
};

void Write(vector<Edge>& res) {
	sort(res.begin(), res.end());
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

			if (min_depth_can_reach[adj] > depth) // bridge found
				res.push_back(Edge(curr, adj));
		}
		else
			min_depth_can_reach[curr] = min(min_depth_can_reach[curr], entry_depth[adj]);
	}
}

int main() {
	vector<vector<int>> data;

	int N;
	while (cin >> N) {
		auto graph = Read(N);

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

	int o; cin >> o;
	return 0;
}
