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

// Explanation:
// Let for example N == 8. For level 1 there are options from 8 to 4. On the top of every of this levels,
// we can put the rest of 8-8=0 bricks, 8-7=1 brick, 8-6=2 bricks, 8-5=3 bricks, 8-4=4 bricks, 
// which corresponds to solutions for N == 0, N == 1, ..., N == 4 respectively. For the last level for even
// N we need to subtract 1 for the base equal to N/2 (but all other options are possible).
ull Solve(int N) {
	if (N == 0) return 0;
	vector<ull> combinations_num(N + 1);
	combinations_num[0] = combinations_num[1] = 1;

	for (int n = 2; n <= N; ++n) {
		ull curr_num = 0;
		
		for (int i = 0; i <= n / 2; ++i)
			curr_num += combinations_num[i];

		// for the even number with last combination base == N/2, we can take combinations_num[N/2] only with bases < N/2.
		// therefore minus one base from the last combination
		if (n % 2 == 0) curr_num -= 1;
		
		combinations_num[n] = curr_num;
	}

	return combinations_num.back();
}

int main() {
	Write(Solve(Read()));
	return 0;
}
