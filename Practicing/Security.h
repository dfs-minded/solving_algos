#include <iostream>
#include <fstream>
#include <vector>
#include <tuple>
#include <set>
#include <algorithm>
#include <numeric>

using namespace std;

struct Event {
	int Point; // stores start if it is a secgment begin event or end if it is an end event.
	int EventType; // 0 for start, 1 for end, -1 for not specified
	int EventId; // number of the segment

	bool operator< (const Event& other) const {
		if (Point < other.Point) return true;
		else if (Point == other.Point) return EventType < other.EventType;
		return false;
	}

	Event(int p, int event_type, int segment_id) : 
		Point(p), EventType(event_type), EventId(segment_id) {}
};

vector<vector<Event>> Read() {
	ifstream input;
	input.open("input.txt");

	vector<vector<Event>> data;

	if (input.is_open()) {
		int N;
		input >> N;
		data.resize(N);

		for (int i = 0; i < N; ++i) {
			int M;
			input >> M;
			for (int j = 0; j < M; ++j) {
				int start; input >> start;
				int end; input >> end;
				data[i].push_back(Event(start, 0, j));
				data[i].push_back(Event(end, 1, j));
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
			output << (res[i] ? "Accepted" : "Wrong Answer") << endl;

		output.close();
	}
}

vector<bool> Solve(vector<vector<Event>> data) {
	vector<bool> res(data.size());

	for (int t = 0; t < data.size(); ++t) {
		sort(data[t].begin(), data[t].end());
		set<int> active_segment_ids;

		// check the ends constraints
		if (data[t].front().Point != 0 || data[t].back().Point < 10000) {
			res[t] = false;
			continue;
		}

		vector<bool> used(data[t].size() / 2);
		for (int i = 0; i < data[t].size(); ++i) {
			auto e = data[t][i];
			if (e.EventType == 0) active_segment_ids.insert(e.EventId); // segment start
			else active_segment_ids.erase(e.EventId); // segment end
			if (active_segment_ids.empty()) { // uncovered time
				res[t] = false;
				break;
			}
			if (active_segment_ids.size() == 1 && 
				(i == data[t].size() - 1 || data[t][i+1].Point != e.Point))
				used[*active_segment_ids.begin()] = true;
		}

		int needed_segments = accumulate(used.begin(), used.end(), 0);
		res[t] = needed_segments == data[t].size() / 2;
	}

	return res;
}

int main() {
	auto data = Read();
	auto res = Solve(data);
	Write(res);

	return 0;
}

