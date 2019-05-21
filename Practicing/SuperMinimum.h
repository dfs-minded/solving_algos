#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <queue>

using namespace std;

pair<vector<int>, int> Read() {
	ifstream input;
	input.open("input.txt");
	int N, K;
	vector<int> data;

	if (input.is_open()) {
		input >> N >> K;
		data.resize(N);
		for (int i = 0; i < N; ++i) input >> data[i];
		input.close();
	}

	return { data, K };
}

void Write(const vector<int>& res) {
	ofstream output("output.txt");
	if (output.is_open()) {
		for (int i = 0; i < res.size(); ++i)
			output << res[i] << " ";
		output.close();
	}
}

vector<int> Solve(const vector<int>& input, int K) {
	vector<int> mins_on_segments;
	deque<int> min_queue;

	for (int l = 0, r = 0; r < input.size();) {
		while (r < input.size() && r - l < K) {
			while (!min_queue.empty() && min_queue.front() > input[r])
				min_queue.pop_front();
			min_queue.push_front(input[r++]);
		}

		mins_on_segments.push_back(min_queue.back());
		if (min_queue.back() == input[l++]) min_queue.pop_back();
	}

	return mins_on_segments;
}

int main() {
	auto[input, K] = Read();
	Write(Solve(input, K));
	return 0;
}
