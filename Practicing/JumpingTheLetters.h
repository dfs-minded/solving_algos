#include <iostream>
#include <fstream>
#include <vector>
#include <tuple>
#include <algorithm>
#include <functional>
#include <string>

using namespace std;

pair<string, int> Read() {
	ifstream input;
	input.open("input.txt");

	string data;
	int K;

	if (input.is_open()) {
		int N;
		input >> N;
		input >> K;
		input >> data;
		
		input.close();
	}

	return { data, K };
}

void Write(int res) {
	ofstream output("output.txt");
	if (output.is_open())
	{
		output << res;
		output.close();
	}
}

// O (26*N)
int Solve(string s, int max_jump) {
	vector<int> last_ind_of(26, -max_jump - 1);
	vector<int> min_price_of(26, numeric_limits<int>::max());
		
	last_ind_of[s[0] - 'A'] = 0;
	min_price_of[s[0] - 'A'] = 0;

	for (int i = 1; i < s.size(); ++i) {
		int char_ascii = s[i] - 'A';
		int curr_min_price = numeric_limits<int>::max();
		// Go through all letters, find which you can reach and take min price you can get.
		for (int j = 0; j < 26; ++j) {
			if (i - last_ind_of[j] > max_jump) continue;

			int curr_price = min_price_of[j] + (char_ascii == j ? 0 : 1);
			curr_min_price = min(curr_min_price, curr_price);
		}
		min_price_of[char_ascii] = curr_min_price;
		last_ind_of[char_ascii] = i;
	}

	return min_price_of[s.back() - 'A'];
}

// O (N*max_jump)
int Solve2(string s, int max_jump) {
	vector<int> res(s.size(), numeric_limits<int>::max() - 100);
	res[0] = 0;

	for (int i = 1; i < s.size(); ++i) {
		for (int j = 1; j <= max_jump && j <= i; ++j) {
			int diff = s[i] == s[i - j] ? 0 : 1;
			res[i] = min(res[i], res[i - j] + diff);
		}
	}

	return res.back();
}


int main() {
	string data;
	int K;

	tie(data, K) = Read();
	Write(Solve(data, K));

	return 0;
}

