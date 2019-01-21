#pragma once

#include <iostream>
#include <map>
#include <unordered_map>
#include <set>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

bool EulerianPathExists(const map<string, pair<int, int>>& in_out_degrees) {
	int different_in_out_count = 0;
	for (auto& key_val : in_out_degrees) {
		if (key_val.second.first != key_val.second.second) {
			++different_in_out_count;
			if (different_in_out_count > 2) return false;
		}
	}
		
	return true;
}

void FindEulerianPath(string v, map<string, unordered_map<string, int>>& graph, vector<string>& res) {
	while (!graph[v].empty()) {
		auto adj_pair = graph[v].begin();
		auto w = adj_pair->first;
		--adj_pair->second;
		if (adj_pair->second == 0)
			graph[v].erase(adj_pair);

		FindEulerianPath(w, graph, res);
	}

	res.push_back(v);
}

bool UntraversedEdgesLeft(const map<string, unordered_map<string, int>>& graph) {
	for (auto& key_val : graph)
		if (!key_val.second.empty()) return true;

	return false;
}

string RestorePassword(vector<string>& path) {
	reverse(path.begin(), path.end());
	string password = path[0];

	for (int i = 1; i < path.size(); ++i)
		password += path[i][1];
	
	return password;
}

bool CheckCornerCases(const map<string, pair<int, int>>& in_out_degree,
	const map<string, unordered_map<string, int>>& graph) {
	if (in_out_degree.size() != 2) return true;
	
	auto first_adj_pair = graph.begin()->second;
	if (graph.size() == 1 && first_adj_pair.begin()->second == 1) return true;

	int num_of_outward_edges = 0;
	auto v = graph.begin()->first;
	auto w = v;
	reverse(w.begin(), w.end());

	if (graph.begin()->second.count(w))
		num_of_outward_edges = graph.begin()->second.at(w);

	int num_of_inward_edges = 0;
	if (graph.count(w) > 0 && graph.at(w).count(v) > 0) {
		num_of_inward_edges = graph.at(w).at(v);
	}

	if (abs(num_of_inward_edges - num_of_outward_edges) > 1
		|| (!num_of_inward_edges && !num_of_outward_edges)) {
		return false;
	}

	return true;
}

int main() {
	map<string, unordered_map<string, int>> graph;
	map<string, pair<int, int>> in_out_degree;

	int N; cin >> N;

	for (int i = 0, num = 0; i < N; ++i) {
		string s; cin >> s;
		string v = s.substr(0, 2);
		string w = s.substr(1, 2);

		++in_out_degree[v].second;
		++in_out_degree[w].first;
		++graph[v][w];
	}

	if (!EulerianPathExists(in_out_degree)) {
		cout << "NO";
		return 0;
	}

	if (!CheckCornerCases(in_out_degree, graph)) {
		cout << "NO";
		return 0;
	}

	vector<string> eulerian_path;
	auto graph_cpy = graph;
	for (auto& key_val : in_out_degree) {
		// start from this vertex which has out degree more than in degree 
		if (key_val.second.second > key_val.second.first) { 
			FindEulerianPath(key_val.first, graph_cpy, eulerian_path);
			break;
		}
	}

	bool untraversed_edges_left = UntraversedEdgesLeft(graph_cpy);

	// out degree <= to in degree in all vertices - pick any as a start
	if (eulerian_path.empty() || untraversed_edges_left) {
		FindEulerianPath(graph.begin()->first, graph, eulerian_path);
		untraversed_edges_left = UntraversedEdgesLeft(graph);
	}

	if (eulerian_path.empty() || untraversed_edges_left) 
		cout << "NO";
	else {
		cout << "YES" << endl;
		cout << RestorePassword(eulerian_path);
	}

	return 0;
}