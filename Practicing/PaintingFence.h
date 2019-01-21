#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

vector<int> Read() {
	ifstream input;
	input.open("input.txt");
	vector<int> data;

	if (input.is_open()) {
		int N; input >> N;
		data.resize(N);

		for (int i = 0; i < N; ++i)
			input >> data[i];

		input.close();
	}

	return data;
}

void Write(int res) {
	ofstream output("output.txt");
	if (output.is_open()) {
		output << res;
		output.close();
	}
}

int Solve(vector<int> heights) {
	if (heights.size() < 5) return 0;
	const int kWidth = 10;
	for (int i = 1; i < heights.size(); ++i)
		heights[i] += heights[i - 1];

	vector<int> max_heights_sum_dp(heights.size(), 0); // max heights sum we can paint if we paint last 5 fences
	max_heights_sum_dp[4] = heights[4];

	for (int i = 5; i < heights.size(); ++i)
		max_heights_sum_dp[i] = max(max_heights_sum_dp[i - 1], // do not paint last fence
			heights[i] - heights[i - 5] + max_heights_sum_dp[i - 5]); // paint last 5 fences

	return max_heights_sum_dp.back() * kWidth;
}

int main() {
	Write(Solve(Read()));
	return 0;
}
