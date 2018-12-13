#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <tuple>
#include <set>
#include <algorithm>
#include <numeric>

using namespace std;

struct Segment {
	int Start;
	int End;
	int Value;

	Segment(int start, int end, int value) : Start(start), End(end), Value(value) {};
};

vector<Segment> Read() {
	ifstream input;
	input.open("input.txt");
	vector<Segment> data;

	if (input.is_open()) {
		int N; input >> N;

		for (int i = 0; i < N; ++i) {
			int start; input >> start;
			int duration; input >> duration;
			int value; input >> value;
			data.push_back(Segment(start, start + duration, value));
		}

		input.close();
	}
	return data;
}

void Write(int max_val, vector<int> res) {
	ofstream output("output.txt");
	if (output.is_open())
	{
		output << max_val << endl;
		output << res.size() << endl;
		for (int i = 0; i < res.size(); ++i)
			output << res[i] + 1 << " ";

		output.close();
	}
}

pair<int, vector<int>> Solve(vector<Segment>& segments) {
	auto cmp = [](const Segment& lhs, const Segment& rhs) { return lhs.End < rhs.End; };
	sort(segments.begin(), segments.end(), cmp);

	vector<pair<int, int>> max_can_get_dp(segments.size()); // sum to the index of the prev element taken 
	max_can_get_dp[0] = { segments[0].Value, -1 };

	for (int i = 1; i < segments.size(); ++i) {
		// don not include curr segment:
		max_can_get_dp[i] = max_can_get_dp[i - 1]; 

		// include curr segment:
		auto include_curr_segment_sum = segments[i].Value;
		int prev_segment_index = -1;
		
		auto pred = []( const Segment& s, const int& start) { 
			return s.End < start; 
		};
		auto first_can_take_iter = lower_bound(segments.begin(), segments.begin() + i + 1, segments[i].Start, pred);
		
		if (first_can_take_iter != segments.begin() + i && first_can_take_iter->End <= segments[i].Start) {
			include_curr_segment_sum += first_can_take_iter->Value;
			prev_segment_index = distance(segments.begin(), first_can_take_iter);
		}

		if (include_curr_segment_sum > max_can_get_dp[i].first)
			max_can_get_dp[i] = { include_curr_segment_sum, prev_segment_index };
	}

	// compose the answer:
	auto max_res_index = distance(max_can_get_dp.begin(), max_element(max_can_get_dp.begin(), max_can_get_dp.end()));
	int max_sum = max_can_get_dp[max_res_index].first;
	
	vector<int> res;
	int prev = max_res_index;
	while (prev != -1) {
		res.push_back(prev);
		prev = max_can_get_dp[prev].second;
	}

	reverse(res.begin(), res.end());
	return { max_sum, res };
}

int main() {
	auto data = Read();

	int max_val;
	vector<int> res;
	tie(max_val, res) = Solve(data);

	Write(max_val, res);

	return 0;
}

