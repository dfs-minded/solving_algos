#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <tuple>

using namespace std;
using ull = unsigned long long;

pair<int, int> Read() {
	ifstream input;
	input.open("input.txt");
	int N, K;

	if (input.is_open()) {
		input >> N >> K;
		input.close();
	}

	return { N, K };
}

void Write(ull res) {
	ofstream output("output.txt");
	if (output.is_open()) {
		output << res;
		output.close();
	}
}

// |N| - number of positions available to place cubes.
// |K| - total number of levels.
// |k_i| - current level.
ull Solve(int N, int K, int k_i, int max_num_cubes, vector<vector<ull>>& memo) {
	if (k_i > K) return 1;
	if (memo[k_i - 1][max_num_cubes])
		return memo[k_i - 1][max_num_cubes];

	int min_num_cubes = 1 + 2 * (K - k_i); // at the current level, to make it to the top
	ull num_ways = 0;

	for (int c = min_num_cubes; c <= max_num_cubes; ++c)
		num_ways += (max_num_cubes - c + 1) * Solve(N - 2, K, k_i + 1, c - 2, memo);

	memo[k_i - 1][max_num_cubes] = num_ways;
	return num_ways;
}

int main() {
	auto[N, K] = Read();

	vector<vector<ull>> memo(K + 1, vector<ull>(N + 1, 0));
	for (int i = 0; i <= N; ++i) memo[0][i] = 1;

	Write(Solve(N - 2, K, 2, N - 2, memo));

	return 0;
}
