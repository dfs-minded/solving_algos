#pragma once

#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

long long KefaAndCompany(vector<pair<int, int>> friends, int d) {
	int begin = 0, end = 0;
	long long res = 0;
	long long curr_sum = 0;

	sort(friends.begin(), friends.end());

	while (end < friends.size()) {
		if (friends[end].first - friends[begin].first >= d) {
			curr_sum -= friends[begin++].second;
		}
		else {
			curr_sum += friends[end++].second;
			res = max(res, curr_sum);
		}
	}

	return res;
}

int main() {
	int n, d;
	cin >> n >> d;

	// pair of money to love
	vector<pair<int, int>> friends(n);
	for (int i = 0; i < n; ++i)
		cin >> friends[i].first >> friends[i].second;

	cout << KefaAndCompany(friends, d);

	int o; cin >> o;
	return 0;
}
