#pragma once

#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int Solve(vector<int>& data) {
	int lo = 0;
	int hi = 1;
	int a = data[lo]; // first val

	while (hi < data.size() && data[hi] == a) ++hi; // find other val, different from a 
	if (hi == data.size()) return data.size();

	int count_a = hi;
	int b = data[hi];
	int count_b = 1;
	bool new_value_occured = false;

	int max_len = hi - lo + 1;

	while (hi < data.size()) {
		if (!new_value_occured && abs(a - b) <= 1) { // keep going if we can extend the range
			++hi;

			if (hi < data.size()) {
				if (data[hi] == a) {
					++count_a;
					max_len = max(max_len, hi - lo + 1);
				}
				else if (data[hi] == b) {
					++count_b;
					max_len = max(max_len, hi - lo + 1);
				}
				else new_value_occured = true;
			}
		} else { // narrow the range from left, till we get rid of one of our variables, 
				 // namely when its count will be zero.
			if (!count_a || !count_b) {
				if (hi < data.size()) {
					if (!count_a) {
						a = data[hi];
						count_a = 1;
					} else {
						b = data[hi];
						count_b = 1;
					}
					new_value_occured = false; // end of processing new value;
				}
			} else { // need to decrease further
				if (data[lo] == a) --count_a;
				else --count_b;

				++lo;
			} 
		} // if (a - b <= 1)
	} // while (hi < data.size())

	return max_len;
}

int main() {
	int N;
	cin >> N;

	vector<int> data(N);
	for (int i = 0; i < N; ++i)
		cin >> data[i];

	cout << Solve(data);

	int o; cin >> o;
}