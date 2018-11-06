#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

// Last index of the ladder, which the corresponding element composes.
vector<int> up_ladders; 
// First index of the ladder, which the corresponding element composes.
vector<int> down_ladders; 

void FindAllLadders(const vector<int>& data) {
	int N = data.size();

	// For up ladders proceed from right to left:
	up_ladders.resize(N);
	up_ladders[N - 1] = N - 1;

	for (int i = N - 1; i > 0; --i) {
		if (data[i] >= data[i - 1]) up_ladders[i - 1] = up_ladders[i];
		else up_ladders[i - 1] = i - 1;
	}

	// For down ladders proceed from left to right:
	down_ladders.resize(N);
	down_ladders[0] = 0;

	for (int i = 1; i < N; ++i) {
		if (data[i] <= data[i - 1]) down_ladders[i] = down_ladders[i - 1];
		else down_ladders[i] = i;
	}
}

string Solve(int lo, int hi) {
	if (up_ladders[lo] == up_ladders[hi] || // pure upwards ladder
		down_ladders[lo] == down_ladders[hi] || // pure downwards ladder
		up_ladders[lo] - down_ladders[hi] >= 0) // mix of up and down
		return "Yes";
	else return "No";
}


int main() {
	int n, m;
	cin >> n >> m;

	vector<int> data(n);
	for (int i = 0; i < n; ++i)
		cin >> data[i];

	FindAllLadders(data);

	for (int i = 0; i < m; ++i) {
		int lo, hi;
		cin >> lo >> hi;
		cout << Solve(lo - 1, hi - 1) << endl;
	}

	//int o; cin >> o;
	return 0;
}
