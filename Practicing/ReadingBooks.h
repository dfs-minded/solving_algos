#pragma once

#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int Books(const vector<int>& time_to_read, int t) {
	int b = 0, e = 0;
	int max_can_read = 0;
	int curr_sum = 0;

	while (e <= time_to_read.size() || curr_sum > t) {
		if (curr_sum <= t) {
			max_can_read = max(max_can_read, e - b);

			if (e < time_to_read.size()) curr_sum += time_to_read[e];
			++e;
		}
		else curr_sum -= time_to_read[b++];
	}

	return max_can_read;
}

int main() {
	int n, t;
	cin >> n >> t;

	vector<int> time_to_read(n);
	for (int i = 0; i < n; ++i)
		cin >> time_to_read[i];

	cout << Books(time_to_read, t);

	int o; cin >> o;
	return 0;
}
