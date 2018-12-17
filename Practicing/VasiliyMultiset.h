#pragma once

#include "Trie.h"

string To32String(int x) {
	string res(32, '0');
	for (int i = 0; i < 32; ++i, x /= 2)
		res[31 - i] += (x & 1);
	return res;
}

int main() {
	Trie<string, char> trie('*');
	trie.Add("0");

	int n; cin >> n;

	for (int i = 0; i < n; ++i) {
		char c; cin >> c;
		int x; cin >> x;
		string x_str = To32String(x);

		switch (c)
		{
		case '+': trie.Add(x_str); break;
		case '-': trie.Delete(x_str); break;
		case '?': 
			auto num_str = trie.BiggestXOR(x_str);
			auto num = stoi(num_str, nullptr, 2);
			cout << (num ^ x) << endl;
			break;
		}
	}

	cin.get();
	return 0;
}