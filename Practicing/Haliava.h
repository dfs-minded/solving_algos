#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <functional>
#include <string>

using namespace std;

vector<int> Read() {
	ifstream input;
	input.open("input.txt");

	vector<int> data(4);

	if (input.is_open()) {
		for (int i = 0; i < 4; ++i)
			input >> data[i];
		
		input.close();
	}

	return data;
}

void Write(int res) {
	ofstream output("output.txt");
	if (output.is_open())
	{
		output << res;
		output.close();
	}
}

struct BuyingState {
	int TShortsBought = 0;
	int Rest = 0;
};

bool operator < (const BuyingState const & lhs, const BuyingState const & rhs) {
	if (lhs.TShortsBought < rhs.TShortsBought) return true;
	else if (lhs.TShortsBought == rhs.TShortsBought && lhs.Rest < rhs.Rest) return true;
	return false;
}

int Solve(const vector<int>& data) {
	int A = data[0] + 1;
	int B = data[1] + 1;
	int C = data[2] + 1;
	int K = data[3];

	// stores a state of taking each coin type i,j,k times -> BuyingState
	vector<vector<vector<BuyingState>>> dp(A, vector<vector<BuyingState>>(B, vector<BuyingState>(C)));

	for (int i = 0; i < A; ++i)
		for (int j = 0; j < B; ++j)
			for (int k = 0; k < C; ++k) {
				if (i + 1 < A) {
					auto new_state = dp[i][j][k];
					new_state.Rest = dp[i][j][k].Rest + 1;
					if (new_state.Rest >= K) { // can buy
						new_state.Rest = 0;
						++new_state.TShortsBought;
					}
					dp[i + 1][j][k] = max(dp[i + 1][j][k], new_state);
				}

				if (j + 1 < B) {
					auto new_state = dp[i][j][k];
					new_state.Rest = dp[i][j][k].Rest + 2;
					if (new_state.Rest >= K) { // can buy
						new_state.Rest = 0;
						++new_state.TShortsBought;
					}
					dp[i][j + 1][k] = max(dp[i][j + 1][k], new_state);
				}

				if (k + 1 < C) {
					auto new_state = dp[i][j][k];
					new_state.Rest = dp[i][j][k].Rest + 3;
					if (new_state.Rest >= K) { // can buy
						new_state.Rest = 0;
						++new_state.TShortsBought;
					}
					dp[i][j][k + 1] = max(dp[i][j][k + 1], new_state);
				}
			}

	return dp.back().back().back().TShortsBought;
}

int main() {
	auto data = Read();
	Write(Solve(data));
}