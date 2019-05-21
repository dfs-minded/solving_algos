#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;
using ull = unsigned long long;

int Read() {
	ifstream input;
	input.open("input.txt");
	int N;

	if (input.is_open()) {
		input >> N;
		input.close();
	}

	return N;
}

void Write(ull res) {
	ofstream output("output.txt");
	if (output.is_open()) {
		output << res;
		output.close();
	}
}

ull Solve(int N) {
	if (N == 0) return 0;
	++N;

	// dp[i][j] : num ways to build a ladder using j cubics
	// i - length of the curr level, 
	// j - number of cubics used
	vector<vector<ull>> dp(N, vector<ull>(N));
	for (int i = 0; i < N; ++i)
		dp[i][i] = 1;

	for (int j = 1; j < N; ++j) // used
		for (int i = 1; i <= j; ++i) // last level	
			for (int k = 1; k <= min(i - 1, N - j - 1); ++k) // next level length
				dp[k][j + k] += dp[i][j];
	
	ull res = 0;
	for (int i = 1; i < N; ++i)
		res += dp[i].back();
	return res;
}

int main() {
	Write(Solve(Read()));
	return 0;
}
