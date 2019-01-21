#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <numeric>
#include <cmath>

using namespace std;
using ull = unsigned long long;

void Read(ull& M, vector<ull>& weight, vector<ull>& cost) {
	ifstream input;
	input.open("input.txt");

	if (input.is_open()) {
		int N; input >> N;
		weight.resize(N);
		cost.resize(N);

		input >> M;

		for (int i = 0; i < N; ++i) input >> weight[i];
		for (int i = 0; i < N; ++i) input >> cost[i];
	}
}

void Write(ull res) {
	ofstream output("output.txt");
	if (output.is_open()) {
		output << res;
		output.close();
	}
}

pair<ull, ull> Sum(const vector<ull>& weight, const vector<ull>& cost, int mask, int offset) {
	ull weight_sum = 0;
	ull cost_sum = 0;
	for (int i = 0; i < 32; ++i) {
		if (mask & (1 << i)) {
			weight_sum += weight[i + offset];
			cost_sum += cost[i + offset];
		}
	}

	return { weight_sum, cost_sum };
}

int Solve(ull M, const vector<ull>& weight, const vector<ull>& cost) {
	ull max_weight = accumulate(weight.begin(), weight.end(), 0ull) - M;

	int N = weight.size();
	ull max_cost = 0;

	// compute all possible baggage combinations for indexes [0, N/2 - 1] and their cost
	vector<pair<ull, int>> weight_and_cost;
	int mid_mask = pow(2, N / 2) - 1;
	for (int mask = 0; mask <= mid_mask; ++mask) {
		auto curr = Sum(weight, cost, mask, 0);
		if (curr.first <= max_weight) {
			max_cost = max(max_cost, curr.second);
			weight_and_cost.push_back(curr);
		}
	}

	int last_first_half = weight_and_cost.size() - 1;
	// compute all possible baggage combinations for indexes [N/2, N - 1] and their cost.
	// update max cost can get combining results from the first and the second half of the items set.
	int last_mask = pow(2, N - N / 2) - 1;
	for (int mask = 0; mask <= last_mask; ++mask) {
		auto curr = Sum(weight, cost, mask, N / 2);

		if (curr.first <= max_weight)
			max_cost = max(max_cost, curr.second);
		else break;

		while (last_first_half >= 0 &&
				(curr.first + weight_and_cost[last_first_half].first > max_weight))
			--last_first_half;

		if (last_first_half >= 0)
			max_cost = max(max_cost, curr.second + weight_and_cost[last_first_half].second);
	}
	
	return accumulate(cost.begin(), cost.end(), 0ull) - max_cost;
}

int main() {
	ull M;
	vector<ull> weight;
	vector<ull> cost;
	Read(M, weight, cost);
	Write(Solve(M, weight, cost));
	return 0;
}