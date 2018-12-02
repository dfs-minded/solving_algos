#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <tuple>
#include <stack>
#include <algorithm>
#include <numeric>

using namespace std;

struct Segment {
	int Start;
	int End;
	int ID;

	bool operator < (const Segment& other) const {
		if (Start < other.Start) return true;
		if (Start == other.Start) return End > other.End;
		return false;
	}

	Segment(int start, int end, int id) : Start(start), End(end), ID(id) {};
};

vector<Segment> Read() {
	ifstream input;
	input.open("input.txt");
	vector<Segment> data;

	if (input.is_open()) {
		int N; input >> N;

		for (int i = 0; i < N; ++i) {
			int start; input >> start;
			int end; input >> end;
			data.push_back(Segment(start, end, i));
		}

		input.close();
	}
	return data;
}

void Write(vector<int> res) {
	ofstream output("output.txt");
	if (output.is_open())
	{
		for (int i = 0; i < res.size(); ++i)
			output << res[i] + 1 << " ";

		output.close();
	}
}

vector<int> Solve(vector<Segment>& segments) {
	vector<int> res(segments.size(), -1);

	sort(segments.begin(), segments.end());
	stack<Segment> bottom_segments;

	for (int i = 0; i < segments.size(); ++i) {
		// remove not appropriate segments
		while (!bottom_segments.empty() && bottom_segments.top().End < segments[i].End) 
			bottom_segments.pop();

		if (!bottom_segments.empty())
			res[segments[i].ID] = bottom_segments.top().ID;

		bottom_segments.push(segments[i]);
	}

	return res;
}

int main() {
	auto data = Read();
	auto res = Solve(data);
	Write(res);

	return 0;
}

