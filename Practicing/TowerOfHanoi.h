#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

int Read() {
	ifstream input;
	input.open("input.txt");
	int N{0};

	if (input.is_open()) {
		input >> N;
		input.close();
	}

	return N;
}

void Write(const vector<vector<int>>& res) {
	ofstream output("output.txt");
	if (output.is_open()) {
		for (int i = 0; i < res.size(); ++i)
			output << res[i][0] << " " << res[i][1] << " " << res[i][2] << endl;
		output.close();
	}
}

void Solve(const int N, int curr, int other, int target, vector<vector<int>>& res) {
	if (N == 1) {
		res.push_back({ N, curr, target });
		return;
	}
	Solve(N - 1, curr, target, other, res);
	res.push_back({ N, curr, target });
	Solve(N - 1, other, curr, target, res);
}

int main() {
	vector<vector<int>> res;
	int N = Read();
	Solve(N, 1, 2, 3, res);
	Write(res);

	return 0;
}
