#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

void Read(vector<int>& first, vector<int>& second) {
	ifstream input;
	input.open("input.txt");

	if (input.is_open()) {
		int N; input >> N;
		first.resize(N);

		int M; input >> M;
		second.resize(M);

		for (int i = 0; i < N; ++i)
			input >> first[i];

		for (int i = 0; i < M; ++i)
			input >> second[i];

		input.close();
	}
}

void Write(int res) {
	ofstream output("output.txt");
	if (output.is_open()) {
		output << res;
		output.close();
	}
}

// Returns index of the first lesser or equal cupboard from the |max_height|.
int FindNearestCanOpen(const vector<pair<int, int>>& cupboard_heights, int curr_cupboard) {
	int max_height = cupboard_heights[curr_cupboard].first - cupboard_heights[curr_cupboard].second;
	auto cmp = [](const pair<int, int>& p, int x) { return p.first < x; };
	auto greater_or_eq_iter = lower_bound(cupboard_heights.begin(), cupboard_heights.end(), max_height, cmp);
	int dist = distance(cupboard_heights.begin(), greater_or_eq_iter);
	if (greater_or_eq_iter->first == max_height) return dist;
	if (greater_or_eq_iter == cupboard_heights.begin()) return -1;
	return dist - 1;
}

vector<pair<int, int>> MergeHeights(const vector<int>& first, const vector<int>& second) {
	vector<int> first_heights = first;
	for (int i = 1; i < first_heights.size(); ++i)
		first_heights[i] += first_heights[i - 1];

	vector<int> second_heights = second;
	for (int i = 1; i < second_heights.size(); ++i)
		second_heights[i] += second_heights[i - 1];

	vector<pair<int, int>> merged_heights;
	for (int k = 0, m = 0; k < first_heights.size() || m < second_heights.size();) {
		if (k == first_heights.size())
			merged_heights.push_back({ second_heights[m], second[m++] });
		else if (m == second_heights.size())
			merged_heights.push_back({ first_heights[k], first[k++] });
		else if (first_heights[k] < second_heights[m])
			merged_heights.push_back({ first_heights[k], first[k++] });
		else if (first_heights[k] > second_heights[m])
			merged_heights.push_back({ second_heights[m], second[m++] });
		else /*==*/
			merged_heights.push_back({ first_heights[k], min(first[k++], second[m++]) });
	}

	return merged_heights;
}

int Solve(const vector<int>& first, const vector<int>& second) {
	auto merged_heights = MergeHeights(first, second);

	vector<int> max_can_open_dp(merged_heights.size());
	max_can_open_dp[0] = 1;
	for (int i = 1; i < max_can_open_dp.size(); ++i) {
		int can_open_below_curr_cupboard = 0;
		int nearest_can_open = FindNearestCanOpen(merged_heights, i);
		if (nearest_can_open != -1) 
			can_open_below_curr_cupboard = max_can_open_dp[nearest_can_open];
		
		max_can_open_dp[i] = max(max_can_open_dp[i - 1] /*do not open curr shelf*/, 
								can_open_below_curr_cupboard + 1 /*open it*/);
	}

	return max_can_open_dp.back();
}

int main() {
	vector<int> first, second;
	Read(first, second);
	Write(Solve(first, second));
	return 0;
}
