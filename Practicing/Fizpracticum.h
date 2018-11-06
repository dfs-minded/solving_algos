#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>

using namespace std;

vector<int> Read() {
	ifstream input;
	input.open("input.txt");
	
	int N;
	vector<int> data;
	
	if (input.is_open()) {
		int N;
		input >> N;
		data.resize(N);

		for (int i = 0; i < N; ++i)
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

int Solve(vector<int> data) {
	sort(data.begin(), data.end());
	int max_sofar = 0;
	int curr = 0;
	for (int begin = 0, end = 0; end < data.size(); ) {
		if (2 * data[begin] >= data[end]) {
			++curr;
			max_sofar = max(max_sofar, curr);
			++end;
		}
		else {
			++begin;
			--curr;
		}
	}
	
	return data.size() - max_sofar;
}

int main() {

	auto data = Read();
	Write(Solve(data));

	return 0;
}

