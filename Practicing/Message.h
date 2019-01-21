#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <functional>

using namespace std;
using ull = unsigned long long;

string Read() {
	ifstream input;
	input.open("input.txt");
	string N;

	if (input.is_open())
		input >> N;

	return N;
}

void Write(string res) {
	ofstream output("output.txt");
	if (output.is_open()) {
		output << res;
		output.close();
	}
}

struct LongNumber {
	ull UpperDigits = 0;
	ull LowerDigits = 0;
	static const ull kBase = 1e9;

	LongNumber(ull upper_digits, ull loewer_digits)
		: UpperDigits(upper_digits), LowerDigits(loewer_digits) {}

	LongNumber operator += (LongNumber other) {
		UpperDigits += other.UpperDigits;
		ull val = LowerDigits + other.LowerDigits;
		UpperDigits += val / kBase;
		LowerDigits = val % kBase;
		return *this;
	}

	LongNumber operator = (LongNumber other) {
		UpperDigits = other.UpperDigits;
		LowerDigits = other.LowerDigits;
		return *this;
	}
};


bool ValidLastSymbol(int from, int to, const string& s) {
	if (s[from] == '0') return false;
	if (s[from] < '3' || (s[from] == '3' && s[to] <= '3')) return true;
	return false;
}

int NumDigits(ull num) {
	int digits_num = 0;
	while (num) {
		num /= 10;
		++digits_num;
	}
	return digits_num;
}

string to_string(LongNumber num) {
	string res = "";
	if (num.UpperDigits) {
		res = to_string(num.UpperDigits);
		int zeroes_inbetween = NumDigits(LongNumber::kBase) - NumDigits(num.LowerDigits) - 1;
		while (zeroes_inbetween) {
			res += '0';
			--zeroes_inbetween;
		}
	}
	res += to_string(num.LowerDigits);
	return res;
}

string Solve(string input) {
	LongNumber f_n_2(0, 1);
	LongNumber f_n_1(0, 1);
	
	for (int i = 1; i < input.size(); ++i) {
		auto f_n = f_n_1;
		if (ValidLastSymbol(i - 1, i, input))
			f_n += f_n_2;

		f_n_2 = f_n_1;
		f_n_1 = f_n;
	}

	return to_string(f_n_1);
}

int main() {
	Write(Solve(Read()));
	return 0;
}