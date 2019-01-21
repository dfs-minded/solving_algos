#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <set>
#include <map>

using namespace std;

struct SegmentEndpoint {
	bool IsBegin;
	int Val = -1;
	int CupboardId = -1;
	int ShelfId = -1;

	SegmentEndpoint(bool is_begin, int val, int cupboard_id, int shelf_id) :
		IsBegin(is_begin), Val(val), CupboardId(cupboard_id), ShelfId(shelf_id) {}

	bool operator < (const SegmentEndpoint& other) {
		return IsBegin > other.IsBegin ? true : Val < other.Val;
	}
};

vector<SegmentEndpoint> Read() {
	ifstream input;
	input.open("input.txt");

	vector<SegmentEndpoint> endpoints;

	if (input.is_open()) {
		int N; input >> N;
		int M; input >> M;

		endpoints.resize(2 * (N + M));
		int height = 0;
		int shelf_height;
		for (int i = 0; i < N; ++i) {
			input >> shelf_height;
			height += shelf_height;
			endpoints[i] = SegmentEndpoint(i % 2 == 0, height, 0, i);
		}

		height = 0;
		for (int i = 0; i < M; ++i) {
			input >> shelf_height;
			height += shelf_height;
			endpoints[N + i] = SegmentEndpoint(i % 2 == 0, height, 1, i);
		}

		input.close();
	}

	return endpoints;
}

void Write(const vector<int>& first, const vector<int>& second) {
	ofstream output("output.txt");
	if (output.is_open()) {
		output << res;
		output.close();
	}
}

int Solve(vector<SegmentEndpoint>& endpoints) {
	sort(endpoints.begin(), endpoints.end());

	set<SegmentEndpoint> closed_segment_ends;	
	vector<SegmentEndpoint> prev_end(endpoints.size() / 2);
	map<SegmentEndpoint, int> max_shelves_dp;
	max_shelves_dp[endpoints[0]] = 1;

	for (int i = 1; i < endpoints.size(); ++i) {
		if (endpoints[i].IsBegin) {
			auto iter = closed_segment_ends.lower_bound(endpoints[i]);
		} else {
			closed_segment_ends.insert(endpoints[i]);
		}
	}
}

int main() {
	auto data = Read();
	Write(Solve(data));
	return 0;
}
