#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

void FillMissingLetters(string* s, int begin, vector<int> letters_count) {
	int next_letter_to_fill = 0;

	for (int i = begin; i < s->size() && next_letter_to_fill < letters_count.size(); ++i) {
		auto& c = s->at(i);
		if (c != '?') continue;

		while (letters_count[next_letter_to_fill] != 0) ++next_letter_to_fill;

		c = 'A' + next_letter_to_fill++;
	}
}

void UpdateLettersLeft(const string& s, int index, const vector<int>& letters_count, int* letters_left) {
	int c = s[index] - 'A';
	if (letters_count[c] == 1) --*letters_left;
	else if (letters_count[c] == 0 || letters_count[c] == 2) ++*letters_left;
}

string EditTheWord(string& s) {
	if (s.size() < 26) return "-1";
	vector<int> letters_count(26);
	
	int letters_left = 26;

	for (int i = 0; i < 26; ++i) {
		if (s[i] == '?') --letters_left;
		else {
			++letters_count[s[i] - 'A'];
			if (letters_count[s[i] - 'A'] == 1) --letters_left;
		}
	}

	for (int begin = 0, end = 26; ; ++begin, ++end) {
		if (letters_left <= 0) {
			auto res = s.substr(begin, end - begin + 1);
			FillMissingLetters(&s, begin, letters_count);
			replace(s.begin(), s.end(), '?', 'A');
			return s;
		}

		if (end == s.size()) break;
		
		if (s[begin] == '?') ++letters_left;
		else {
			--letters_count[s[begin] - 'A'];
			UpdateLettersLeft(s, begin, letters_count, &letters_left);
		}
		if (s[end] == '?') --letters_left;
		else {
			++letters_count[s[end] - 'A'];
			UpdateLettersLeft(s, end, letters_count, &letters_left);
		}
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