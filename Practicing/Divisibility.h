#pragma once

#include <iostream>
#include <vector>

using namespace std;

int solveDivisibility(const vector<int>& input) {
	const int kModulo = 1e9 + 7;

	vector<int> dp(input.size() + 1); // stores the sum of all subsequences of length i, i=[1,N], 
									  // which end at index i-1 of the input array.	
	dp[0] = 1;
	for (int i = 0; i < input.size(); ++i) {
		for (int len = i + 1; len > 0; --len) {
			if (input[i] % len == 0) dp[len] += dp[len - 1];
			dp[len] %= kModulo;
		}
	}
	
	int res = 0;
	for (int i = 0; i < dp.size(); ++i) {
		res += dp[i];
		res %= kModulo;
	}
	return res - 1; // minus extra one from dp[0]
}

int main() {
	int N;
	cin >> N;
	vector<int> input(N);

	for (int i = 0; i < N; ++i)
		cin >> input[i];

	cout << solveDivisibility(input);
}