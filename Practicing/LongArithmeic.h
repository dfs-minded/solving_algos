#pragma once
#include <string>
#include <algorithm>

using namespace std;

using ull = unsigned long long;

string Add(const string& s, const string& num) {
	const int kBase = 10;
	const int kZeroAscii = 48;
	string res;
	res.resize(max(s.size(), num.size()) + 1);
	int carry = 0;

	size_t k = res.size() - 1;
	for (size_t i = s.size() - 1, j = num.size() - 1; i || j;) {
		if (i >= 0 && j >= 0) {
			int val = (s[i--] - kZeroAscii) + (num[j--] - kZeroAscii) + carry;
			carry = val / kBase;
			val %= kBase;
			res[k--] = '0' + val;
		}
		else if (i >= 0) res[k--] = s[i--];
		else res[k--] = num[j--];
	}

	res[k--] = '0' + carry;
	if (res[0] == '0') res.erase(res.begin());
	return res;
}

struct LongNumber {
	ull UpperDigits = 0;
	ull LowerDigits = 0;
	static const ull kBase = 1e2;

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

int main() {
	LongNumber n1(1, 1);
	LongNumber n2(1, 1);
	n1 += n2;

	cout << to_string(n1);

	cin.get();
	return 0;
}