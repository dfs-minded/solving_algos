#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <tuple>

using namespace std;

pair<int, vector<int>> Read() {
	ifstream input;
	input.open("input.txt");

	int N, K;
	vector<int> data;

	if (input.is_open()) {
		int N;
		input >> N;
		data.resize(N);

		input >> K;

		for (int i = 0; i < N; ++i)
			input >> data[i];

		input.close();
	}

	return { K, data };
}

void Write(int res) {
	ofstream output("output.txt");
	if (output.is_open())
	{
		output << res;
		output.close();
	}
}

bool CanAccomodate(int cows_num, const vector<int>& houses, int min_dist) {
	int accomodated_cows = 1;

	for (int l = 0, r = 1; r < houses.size(); ++r) {
		if (houses[r] - houses[l] >= min_dist) {
			++accomodated_cows;
			l = r;
		}
	}

	return accomodated_cows >= cows_num;
}

int Solve(int cows_num, const vector<int>& houses) {
	int lo = 0;
	int hi = houses.back() - houses.front();

	while (hi - lo > 1) {
		int mid = lo + (hi - lo) / 2;
		if (CanAccomodate(cows_num, houses, mid)) lo = mid;
		else hi = mid;
	}

	return lo;
}

int main() {
	int cows_num;
	vector<int> houses;
	
	tie(cows_num, houses) = Read();
	Write(Solve(cows_num, houses));

	return 0;
}