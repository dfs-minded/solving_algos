#include <iostream>
#include <fstream>
#include <vector>
#include <tuple>
#include <set>
#include <algorithm>
#include <numeric>

using namespace std;

enum class EventType { START, END, POINT };

struct Event {
	int Point; // stores start if it is a secgment begin event or end if it is an end event.
	EventType Type;
	int EventId; // sequential number of an event

	bool operator< (const Event& other) const noexcept {
		if (Point < other.Point) return true;
		else if (Point == other.Point) {
			if (Type == other.Type) return EventId > other.EventId;
			if (Type == EventType::START) return true;
			if (Type == EventType::POINT) return other.Type == EventType::END;
		}
		return false;
	}

	Event(int p, EventType event_type, int event_id) :
		Point(p), Type(event_type), EventId(event_id) {}
};

vector<Event> Read(int& points_num) {
	ifstream input;
	input.open("input.txt");
	vector<Event> data;

	if (input.is_open()) {
		int N;
		input >> N >> points_num;

		for (int i = 0; i < N; ++i) {
			int first; input >> first;
			int second; input >> second;
			data.push_back(Event(min(first, second), EventType::START, i));
			data.push_back(Event(max(first, second), EventType::END, i));
		}

		for (int j = 0; j < points_num; ++j) {
			int p; input >> p;
			data.push_back(Event(p, EventType::POINT, j));
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
			output << res[i] << " ";

		output.close();
	}
}

vector<int> Solve(vector<Event>& events, int points_num) {
	vector<int> res(points_num);
	set<int> active_segments;

	sort(events.begin(), events.end());

	for (auto e : events) {
		switch (e.Type) {
		case EventType::POINT:
			res[e.EventId] = active_segments.size();
			break;
		case EventType::START:
			active_segments.insert(e.EventId);
			break;
		case EventType::END:
			active_segments.erase(e.EventId);
			break;
		default: break;
		}
	}

	return res;
}

int main() {
	int points_num;
	auto data = Read(points_num);
	auto res = Solve(data, points_num);
	Write(res);

	return 0;
}

