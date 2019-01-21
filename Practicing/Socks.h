#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

void Read(vector<int>& queries, vector<int>& socks) {
	ifstream input;
	input.open("input.txt");

	if (input.is_open()) {
		int L; input >> L;
		int N; input >> N;
		int M; input >> M;

		// store sock begin and end as an index in the array, +1 for sock begin, -1 for sock end.
		socks.resize(L + 2);
		for (int i = 0; i < N; ++i) {
			int begin, end;
			input >> begin >> end;
			++socks[begin];
			--socks[end + 1];
		}

		// store queries in the order as they come
		for (int i = 0; i < M; ++i) {
			int point;
			input >> point;
			queries.push_back(point);
		}

		input.close();
	}
}

void Write(const vector<int>& res) {
	ofstream output("output.txt");
	if (output.is_open()) {
		for (int i = 0; i < res.size(); ++i)
			output << res[i] << endl;
		output.close();
	}
}

// Answers are stored in |queries|
void Solve(vector<int>& queries, const vector<int>& socks) {
	// sort queries using key-index marks
	vector<int> queries_sorted(socks.size());
	for (int i = 0; i < queries.size(); ++i)
		queries_sorted[queries[i]] = 1;

	int balance = 0;
	for (int i = 0; i < socks.size(); ++i) {
		balance += socks[i];
		if (queries_sorted[i]) 
			queries_sorted[i] = balance; // update number of socks for each query
	}

	// collect calculated results in the order, they appeared in the input 
	// and store them at the same place in the |queries| array.
	for (int i = 0; i < queries.size(); ++i)
		queries[i] = queries_sorted[queries[i]];
}

int main() {
	vector<int> queries, socks;
	Read(queries, socks);
	Solve(queries, socks);
	Write(queries);
	return 0;
}
