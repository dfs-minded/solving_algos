#pragma once

struct Interval {
	int start;
	int end;
	int weight;
	Interval() : start(0), end(0) {}
	Interval(int s, int e) : start(s), end(e) {}
};

vector<int> CountIntersectionsWithPoints(vector<Interval>& intervals, vector<int> points) {
	auto cmp = [](Interval& lhs, Interval& rhs) { return lhs.start < rhs.start; };
	sort(intervals.begin(), intervals.end(), cmp);
	sort(points.begin(), points.end());

	vector<int> res(points.size());
	stack<Interval> curr_intervals;

	for (int p_i = 0, j = 0; p_i < points.size() && j < intervals.size(); ++p_i) {
		// remove all intervals from stack, which end before current point
		while (!curr_intervals.empty() && curr_intervals.top().end < points[p_i])
			curr_intervals.pop();
		
		// add all intersecting intervals to the stack
		while (j < intervals.size() && intervals[j].start <= points[p_i]) {
			if (intervals[j].end <= points[p_i]) 
				curr_intervals.push(intervals[j]);
			++j;
		}

		res[p_i] = curr_intervals.size();
	}

	return res;
}

bool NeedAllIntervalsToCover(vector<Interval>& intervals, Interval& LEN) {
	auto cmp = [](Interval& lhs, Interval& rhs) { return lhs.start < rhs.start; };
	sort(intervals.begin(), intervals.end(), cmp);

	for (int i = 0; i < intervals.size(); ++i) {
		if (intervals[i].end < LEN.start || intervals[i].start > LEN.end) 
			return false;
		if (i > 0 && intervals[i - 1].end >= intervals[i].start) 
			return false;
	}

	return true;
}

int MaxDisjointSegmentsSum(vector<Interval>& intervals) {
	auto cmp = [](Interval& lhs, Interval& rhs) { return lhs.end < rhs.end; };
	sort(intervals.begin(), intervals.end(), cmp);

	vector<int> max_sum(intervals.size()); // stores the maximum sum we can collect if we include i-th
										   // segment into result.

	for (int i = 0; i < intervals.size(); ++i) {
		for (int j = 0; j < i; ++j) {
			if (intervals[j].end < intervals[i].start)
				dp[i] = max(dp[i], dp[j]);
			else break;
		}
		dp[i] += intervals[i].weight;
	}

	return max_element(max_sum.begin(), max_sum.end());
}

vector<int> CompletePossession(vector<Interval>& intervals) {
	vector<int> res(intervals.size(), -1);
	return res;
}