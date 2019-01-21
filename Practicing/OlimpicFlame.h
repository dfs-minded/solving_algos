#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <numeric>
#include <cmath>
#include <tuple>

using namespace std;
using ull = unsigned long long;

void Read(int& M, int& K, vector<int>& dist) {
	ifstream input;
	input.open("input.txt");

	if (input.is_open()) {
		int N; input >> N;
		input >> M;
		input >> K;

		dist.resize(N);
		for (int i = 0; i < N; ++i) input >> dist[i];
	}
}

void Write(int saved_fakels, const vector<pair<int, int>>& merged_intervals) {
	ofstream output("output.txt");
	if (output.is_open()) {
		output << saved_fakels << endl;
		output << merged_intervals.size() << endl;

		for (int i = 0; i < merged_intervals.size(); ++i)
			output << merged_intervals[i].first << " " << merged_intervals[i].second << endl;
		
		output.close();
	}
}

pair<int, vector<pair<int, int>>> Solve(int M, int K, const vector<int>& dist) {
	vector<pair<int, int>> merged; // pair of group id to num merged
	int left_groups_count = 0;

	// greedily merge intervals:
	int curr_dist = 0;
	int num_meged = 0;

	for (int i = 0; i < dist.size() || curr_dist; ++i) {
		if (i < dist.size() && curr_dist + dist[i] <= K && num_meged < M) {
			curr_dist += dist[i];
			++num_meged;
		} else {
			if (num_meged > 1) {
				merged.push_back({ i + 1 - num_meged, num_meged });
				++left_groups_count;
			}

			curr_dist = i < dist.size() ? dist[i] : 0;
			num_meged = 1;
		}

		if (curr_dist >= K && num_meged == 1) ++left_groups_count; // one element group
	}

	int saved_fakels = dist.size() - left_groups_count;
	return { saved_fakels, merged };
}

int main() {
	int M, K;
	vector<int> dist;
	Read(M, K, dist);

	int saved_fakels;
	vector<pair<int, int>> merged_intervals;
	tie(saved_fakels, merged_intervals) = Solve(M, K, dist);

	Write(saved_fakels, merged_intervals);
	return 0;
}