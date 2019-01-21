#include <iostream>
#include <fstream>
#include <vector>
#include <tuple>
#include <algorithm>
#include <functional>

using namespace std;

pair<vector<int>, int> Read() {
	ifstream input;
	input.open("input.txt");

	vector<int> data;
	int S;

	if (input.is_open()) {
		int N;
		input >> N;
		data.resize(N);

		for (int i = 0; i < N; ++i)
			input >> data[i];

		input >> S;
		input.close();
	}

	return { data, S };
}

void Write(vector<int> res) {
	ofstream output("output.txt");
	if (output.is_open())
	{
		for (auto val : res)
			output << val << " ";
		
		if (res.empty()) output << "No solution";

		output.close();
	}
}

vector<int> Solve(vector<int> values, int target_sum) {
	vector<int> count_dp(target_sum + 1, target_sum + 1);
	count_dp[0] = 0;
	vector<int> last_added_dp(target_sum + 1, -1);

	for (int s = 1; s <= target_sum; ++s) {
		for (int i = 0; i < values.size(); ++i) {
			if (s - values[i] >= 0 && count_dp[s] > count_dp[s - values[i]] + 1) {
				count_dp[s] = count_dp[s - values[i]] + 1;
				last_added_dp[s] = i;
			}
		}
	}

	vector<int> change;
	int last_added = last_added_dp[target_sum];
	while (last_added != -1) {
		change.push_back(values[last_added]);
		target_sum -= values[last_added];
		last_added = last_added_dp[target_sum];
	}

	return change;
}

int main() {
	vector<int> data;
	int S;

	tie(data, S) = Read();
	Write(Solve(data, S));

	return 0;
}