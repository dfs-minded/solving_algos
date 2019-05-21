class Solution {
public:
	vector<int> sumOfDistancesInTree(int N, vector<vector<int>>& edges) {
		graph_ = ToAdjList(N, edges);
		subtree_size_ = vector<int>(N);
		total_dist_ = vector<int>(N);

		int sum = 0;
		ComputeSubtreeSizes(0, -1, 1, sum);
		CalcSums(0, -1, sum);

		return total_dist_;
	}

private:
	vector<vector<int>> graph_;
	vector<int> subtree_size_;
	vector<int> total_dist_;

	vector<vector<int>> ToAdjList(int N, vector<vector<int>>& edges) {
		vector<vector<int>> graph(N);

		for (auto& e : edges) {
			graph[e[0]].push_back(e[1]);
			graph[e[1]].push_back(e[0]);
		}

		return graph;
	}

	// depth of child nodes
	void ComputeSubtreeSizes(int node, int parent, int depth, int& sum) {
		++subtree_size_[node];

		for (auto adj : graph_[node]) {
			if (adj == parent) continue;
			ComputeSubtreeSizes(adj, node, depth + 1, sum);
			subtree_size_[node] += subtree_size_[adj];
			sum += depth;
		}
	}

	void CalcSums(int node, int parent, int curr_sum) {
		total_dist_[node] = curr_sum;

		for (auto adj : graph_[node]) {
			if (adj == parent) continue;
			int adj_dist_sum = curr_sum + subtree_size_[node] - 2 * subtree_size_[adj];
			subtree_size_[adj] += subtree_size_[node] - subtree_size_[adj];
			CalcSums(adj, node, adj_dist_sum);
		}
	}
};

int main() {
	Solution sln;
	
	vector<vector<int>> edges { {0, 1}, { 0,2 }, { 2,3 }, { 2,4 }, { 2,5 }};
	auto res = sln.sumOfDistancesInTree(6, edges);

	cin.get();
	return 0;
}