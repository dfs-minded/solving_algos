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
	int Score;
	int ID;

	Segment(int start, int end, int score, int id) : Start(start), End(end), Score(score), ID(id) {};

	bool operator < (const Segment& other) {
		return End == other.End ? Score > other.Score : End < other.End;
	}
};

auto LowerBoundPred = [](const Segment& s, int val) { return s.End < val; };


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
			data.push_back(Segment(start, start + duration, value, i + 1));
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
			output << res[i] << " ";

		output.close();
	}
}

int NearestCanTake(const vector<Segment>& segments, int i) {
	int val = segments[i].Start;
	auto greater_or_eq_end_iter = lower_bound(segments.begin(), segments.end(), val, LowerBoundPred);

	if (greater_or_eq_end_iter == segments.begin())
		return greater_or_eq_end_iter->End > val ? -1 : 0;
	
	int dist = distance(segments.begin(), greater_or_eq_end_iter);
	return (greater_or_eq_end_iter->End == val) ? dist : dist - 1;
}

pair<int, vector<int>> Solve(vector<Segment>& segments) {
	vector<int> max_scores_dp(segments.size());
	max_scores_dp[0] = segments[0].Score;

	vector<int> last_segment_index(segments.size(), -1);
	last_segment_index[0] = 0;

	sort(segments.begin(), segments.end());

	for (int i = 1; i < segments.size(); ++i) {
		int index = NearestCanTake(segments, i);
		int prev_ma_score = (index == -1) ? 0 : max_scores_dp[index];
		
		// do not take curr segment:
		max_scores_dp[i] = max_scores_dp[i - 1];
		last_segment_index[i] = last_segment_index[i - 1];

		// take curr segment:
		if (prev_ma_score + segments[i].Score > max_scores_dp[i]) {
			max_scores_dp[i] = prev_ma_score + segments[i].Score;
			last_segment_index[i] = i;
		}
	}

	vector<int> task_ids;
	int last_task_index = last_segment_index.back();

	while (true) {
		task_ids.push_back(segments[last_task_index].ID);
		auto val = max_scores_dp[last_task_index] - segments[last_task_index].Score;
		if (val == 0) break;

		auto prev_segm_iter = lower_bound(segments.begin(), segments.begin() + last_task_index + 1, 
											val, LowerBoundPred);

		last_task_index = distance(segments.begin(), prev_segm_iter);
	}

	reverse(task_ids.begin(), task_ids.end());

	return { max_scores_dp.back(), task_ids };
}

int main() {
	auto data = Read();

	int max_val;
	vector<int> res;
	tie(max_val, res) = Solve(data);

	Write(max_val, res);

	return 0;
}

