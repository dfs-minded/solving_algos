#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

vector<vector<int>> Read() {
	ifstream input;
	input.open("input.txt");
	int N, M;
	vector<vector<int>> data;

	if (input.is_open()) {
		input >> N >> M;
		data.resize(N);

		for (int r = 0; r < N; ++r) {
			data[r].resize(M);

			for (int c = 0; r < N; ++r)
				cin >> data[r][c];
		}


		input.close();
	}

	return data;
}

void Write(long long sum, int x1, int y1, int x2, int y2) {
	ofstream output("output.txt");
	if (output.is_open()) {
		output << sum << endl;
		output << x1 << " " << y1 << " " << x2 << " " << y2;
		output.close();
	}
}

tuple<int, int, int, int, int> Solve(vector<vector<int>> data) {
	int best_sum = numeric_limits<int>::min();
	int x1, y1, x2, y2;

	int R = data.size();
	int C = data[0].size();

	for (int l = 0; l < C - 1; ++l) {
		for (int r = l + 1; r < C; ++r) {
			int prev_best_sum = numeric_limits<int>::min();

			for (int h = R - 2; h >= 0; --h) {
				auto curr_sum = data[h][r];
				if (l - 1 >= 0) curr_sum -= data[h][l - 1];

				if (prev_best_sum == numeric_limits<int>::min()) {
					prev_best_sum = curr_sum + data[h + 1][r];
					if (l - 1 >= 0) prev_best_sum -= data[h + 1][l - 1];
				}
			}
		}
	}

	return { best_sum, x1, y1, x2, y2 };
}

int main() {
	auto[sum, x1, y1, x2, y2] = Solve(Read());
	Write(sum, x1, y1, x2, y2);

	return 0;
}
