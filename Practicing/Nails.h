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

int Solve(vector<int> values, int N) {
	if (values.size() <= 1) return 0;
	sort(values.begin(), values.end());
	vector<int> res(N - 1);
	res[1] = (values[1] - values[0]);

	for (int i = 2; i < res.size(); ++i) {
		// min from value in case I connect the i-th nail to previous one
		// and in case we do not connect to prev, but then prev needs to be connected.
		res[i] = min(res[i-1] + (values[i] - values[i-1]), 
					 res[i-2] + (values[i-1] - values[i-2]));
	}

	return res.back() + (values[N-1] - values[N-2]);
}


int main() {
	vector<int> data;
	int N;

	tie(data, N) = Read();
	Write(Solve(data, N));

	return 0;
}

