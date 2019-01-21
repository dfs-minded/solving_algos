#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

vector<char> Read() {
	ifstream input;
	input.open("input.txt");

	vector<char> data;
	if (input.is_open()) {
		int N; input >> N;
		data.resize(N);

		for (int i = 0; i < N; ++i)
			input >> data[i];

		input.close();
	}

	return data;
}


using ull = unsigned long long;

void Write(ull res) {
	ofstream output("output.txt");
	if (output.is_open()) {
		output << res;
		output.close();
	}
}

ull SolveN_2(const vector<char>& data) {
	vector<int> balance_prefix_sums(data.size() + 1);
	for (int i = 1; i < balance_prefix_sums.size(); ++i) {
		int curr_person = data[i - 1] == 'a' ? 1 : -1;
		balance_prefix_sums[i] += balance_prefix_sums[i - 1] + curr_person;
	}
	
	ull groups_can_select = 0;
	for (int start = 0; start < balance_prefix_sums.size(); ++start)
		for (int end = start + 2; end < balance_prefix_sums.size(); end += 2)
			if (balance_prefix_sums[end] - balance_prefix_sums[start] == 0)
				++groups_can_select;
		
	return groups_can_select;
}


ull Solve(const vector<char>& data) {
	int N = data.size();
	vector<int> existing_prefixes(2*N + 1);
	existing_prefixes[N] = 1;

	ull num_posibilities = 0;
	int curr_balance = 0;
	for (int i = 0; i < N; ++i) {
		curr_balance += (data[i] == 'a') ? 1 : -1;
		num_posibilities += existing_prefixes[N + curr_balance];
		++existing_prefixes[N + curr_balance];
	}

	return num_posibilities;
}

int main() {
	Write(Solve(Read()));
	return 0;
}
