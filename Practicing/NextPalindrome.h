#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <utility>

using namespace std;

string Read() {
	ifstream input;
	input.open("input.txt");
	string N;

	if (input.is_open()) {
		input >> N;
		input.close();
	}

	return N;
}

void Write(string res) {
	ofstream output("output.txt");
	if (output.is_open()) {
		output << res << endl;
		output.close();
	}
}

void PlusOne(string& input) {
	const int kBase = 10;
	int carry = 1;

	for (int i = input.size() - 1; i >= 0; --i) {
		int val = (input[i] - '0') + carry;
		carry = val / kBase;
		val %= kBase;
		input[i] = '0' + val;
	}

	if (carry) input = '1' + input;
 }

string Solve(string x_str) {
	if (x_str.length() == 1) return "11";

	auto mid = x_str.size() / 2;
	auto l = mid - 1;
	auto r = x_str.size() % 2 == 0 ? mid : mid + 1;

	auto x_left_str = x_str.substr(0, mid); // sub-string on the left from center
	auto x_right_str = x_str.substr(r, mid); // sub-string on the right from center
	
	auto x_left_str_reversed = x_left_str;
	reverse(x_left_str_reversed.begin(), x_left_str_reversed.end());
	
	if (x_str.size() % 2) { // odd length
		if (x_left_str_reversed <= x_right_str) {
			x_left_str += x_str[mid];
			PlusOne(x_left_str);

			auto res_right = x_left_str.substr(0, x_left_str.size() - 1);
			reverse(res_right.begin(), res_right.end());
			return x_left_str + res_right;
		}

		return x_left_str + x_str[mid] + x_left_str_reversed;
	}
		
	// even length:
	if (x_left_str_reversed <= x_right_str) {
		PlusOne(x_left_str);

		if (x_left_str.size() > x_right_str.size()) { // 9 + 1 = 10, got odd res length after incrementing
			auto res_right = x_left_str.substr(0, x_left_str.size() - 1);
			reverse(res_right.begin(), res_right.end());
			return x_left_str + res_right;
		}
	}

	auto res_right = x_left_str;
	reverse(res_right.begin(), res_right.end());
	return x_left_str + res_right;
}

int main() {
	Write(Solve(Read()));
	return 0;
}