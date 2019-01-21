#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

vector<vector<int>> Read() {
	ifstream input;
	input.open("input.txt");
	vector<vector<int>> data;

	if (input.is_open()) {
		int N; input >> N;
		data.resize(N);

		for (int i = 0; i < N; ++i) {
			data[i].resize(3);
			input >> data[i][0];
			input >> data[i][1];
			input >> data[i][2];
		}

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

int Solve(const vector<vector<int>>& time_to_buy) {
	if (time_to_buy.empty()) return 0;
	// min time first i person need to buy tickets
	vector<int> min_time_needed(time_to_buy.size(), numeric_limits<int>::max()); 

	min_time_needed[0] = time_to_buy[0][0];
	if (time_to_buy.size() == 1) return min_time_needed[0];

	min_time_needed[1] = min(time_to_buy[1][0] + min_time_needed[0], time_to_buy[0][1]);
	if (time_to_buy.size() == 2) return min_time_needed[1];

	min_time_needed[2] = min({ time_to_buy[2][0] + min_time_needed[1],
								time_to_buy[1][1] + min_time_needed[0], time_to_buy[0][2] });

	for (int i = 3; i < time_to_buy.size(); ++i) {
		min_time_needed[i] = min({ time_to_buy[i][0] + min_time_needed[i - 1], // buy yourself
								   time_to_buy[i - 1][1] + min_time_needed[i - 2], // prev person buys you
								   time_to_buy[i - 2][2] + min_time_needed[i - 3]  // prev prev person buys you
								});
	}

	return min_time_needed.back();
}

int main() {
	Write(Solve(Read()));
	return 0;
}
