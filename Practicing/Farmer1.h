#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <functional>
#include <string>
#include <stack>

using namespace std;

vector<vector<int>> Read() {
	ifstream input;
	input.open("input.txt");

	vector<vector<int>> data;

	if (input.is_open()) {
		int N;
		input >> N;
		data.resize(N);

		for (int r = 0; r < N; ++r) {
			data[r].resize(N);
			string row;
			input >> row;
			for (int c = 0; c < N; ++c)
				data[r][c] = row[c] - '0';
		}
		input.close();
	}

	return data;
}

void Write(int res) {
	ofstream output("output.txt");
	if (output.is_open())
	{
		output << res;
		output.close();
	}
}

vector<vector<int>> NearestMinUp(const vector<vector<int>>& data) {
	vector<vector<int>> res(data.size(), vector<int>(data[0].size(), -1));

	for (int c = 0; c < data[0].size(); ++c) {
		stack<int> non_decreasing;
		for (int r = 0; r < data.size(); ++r) {
			while (!non_decreasing.empty() && data[non_decreasing.top()][c] >= data[r][c])
				non_decreasing.pop();
			
			if (!non_decreasing.empty()) res[r][c] = non_decreasing.top();
			non_decreasing.push(r);
		}
	}

	return res;
}

vector<vector<int>> NearestMinDown(const vector<vector<int>>& data) {
	vector<vector<int>> res(data.size(), vector<int>(data[0].size(), data.size()));

	for (int c = 0; c < data[0].size(); ++c) {
		stack<int> non_decreasing;
		for (int r = data.size() - 1; r >= 0; --r) {
			while (!non_decreasing.empty() && data[non_decreasing.top()][c] >= data[r][c])
				non_decreasing.pop();

			if (!non_decreasing.empty()) res[r][c] = non_decreasing.top();
			non_decreasing.push(r);
		}
	}

	return res;
}

// data is squared
int Solve(vector<vector<int>>& data) {
	int N = data.size();
	
	// calc prefix sums
	for (int r = 0; r < N; ++r) {
		for (int c = 1; c < N; ++c) {
			data[r][c] += data[r][c - 1] * data[r][c];
		}
	}

	auto nearest_min_up = NearestMinUp(data);
	auto nearest_min_down = NearestMinDown(data);

	int max_area_sofar = 0;
	for (int r = 0; r < N; ++r) {
		for (int c = 0; c < N; ++c) {
			int width = data[r][c];
			int height = nearest_min_down[r][c] - nearest_min_up[r][c] - 1;
			int side_of_square = min(width, height);
			max_area_sofar = max(max_area_sofar, side_of_square*side_of_square);
		}
	}

	return max_area_sofar;
}

int main() {
	auto data = Read();
	Write(Solve(data));

	return 0;
}