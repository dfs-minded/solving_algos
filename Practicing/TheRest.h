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
	int M;

	if (input.is_open()) {
		int N;
		input >> N;
		data.resize(N);

		for (int i = 0; i < N; ++i)
			input >> data[i];

		input >> M;
		input.close();
	}

	return { data, M };
}

void Write(int res) {
	ofstream output("output.txt");
	if (output.is_open())
	{
		output << res;
		output.close();
	}
}

int Solve(vector<int> values, int target) {
	vector<int> res(target + 1, numeric_limits<int>::max() - 1);
	res[0] = 0;

	for (int i = 1; i < res.size(); ++i) {
		for (int j = 0; j < values.size(); ++j) {
			if (i >= values[j])
				res[i] = min(res[i], res[i - values[j]] + 1);
		}
	}

	return res.back() == numeric_limits<int>::max() - 1 ? -1 : res.back();
}


int main() {
	vector<int> data;
	int N;

	tie(data, N) = Read();
	Write(Solve(data, N));

	return 0;
}

