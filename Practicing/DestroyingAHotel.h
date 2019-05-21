#pragma once

#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int PassangersDeliveryTime(const vector<pair<int, int>>& passangers_arrive_times) {
	int total_time_needed = 0;
	int last_level = passangers_arrive_times.size();

	for (auto& level_time_pair : passangers_arrive_times) {
		auto[level, time] = level_time_pair;

	}

	return total_time_needed;
}

int main() {
	int N, levels_num;
	cin >> N >> levels_num;
	vector<pair<int, int>> input(N);

	for (int i = 0; i < N; ++i)
		cin >> input[i].first >> input[i].second;

	cout << PassangersDeliveryTime(input);
	return 0;
}