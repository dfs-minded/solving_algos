#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <tuple>
#include <algorithm>
#include <functional>

using namespace std;

vector<vector<pair<int,int>>> Read() {
	ifstream input;
	input.open("input.txt");

	vector<vector<pair<int,int>>> data;
	int N;

	if (input.is_open()) {
		int N;
		input >> N;
		data.resize(N);

		int M;
		for (int i = 0; i < N; ++i) {
			cin >> M;
			data[i].resize(M);
			for (int j = 0; j < M; ++j) {
				cin >> data[i][j].first;
				cin >> data[i][j].second;
			}
		}

		input.close();
	}

	return data;
}

void Write(vector<bool> res) {
	ofstream output("output.txt");
	if (output.is_open())
	{
		for (int i = 0; i < res.size(); ++i)
			output << res[i] ? "Accepted" : "Wrong Answer";
		output.close();
	}
}

vector<bool> Solve(const vector<vector<pair<int, int>>>& input) {
	vector<bool> res(input.size());

	return res;
}

int main() {
	auto data = Read();
	auto res = Solve(data);
	Write(res);
	return 0;
}