#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

void FillMissingLetters(string* s, int begin, vector<int> letters_count) {
	int next_letter_to_fill = 0;

	for (int i = 0; i < 26; ++i) {
		auto& c = s->at(begin + i);
		if (c != '?') continue;

		while (letters_count[next_letter_to_fill] != 0) ++next_letter_to_fill;

		c = 'A' + next_letter_to_fill++;
	}

	replace(s->begin(), s->end(), '?', 'A');
}

string EditTheWord(string& s) {
	if (s.size() < 26) return "-1";
	vector<int> letters_count(26);
	
	int letters_left = 26;

	for (int begin = 0, end = 0; end < s.size() || letters_left == 0; ++end) {
		if (letters_left == 0) {
			FillMissingLetters(&s, begin, letters_count);
			return s;
		}

		if (s[end] == '?') --letters_left;
		else {
			++letters_count[s[end] - 'A'];
			if (letters_count[s[end] - 'A'] == 1) --letters_left;
		}

		if (end - begin < 26) continue;

		if (s[begin] == '?') ++letters_left;
		else {
			--letters_count[s[begin] - 'A'];
			if (letters_count[s[begin] - 'A'] == 0) ++letters_left;
		}

		++begin;
	}

	return "-1";
}

int main() {
	string input;
	cin >> input;

	cout << EditTheWord(input);

	//int u; cin >> u;
	return 0;
}