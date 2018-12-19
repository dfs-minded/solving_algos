#include <string>
#include <iostream>

#include "Trie.h"

using namespace std;

string To32String(int x) {
	const int kSize = 32;
	string res(kSize, '0');
	for (int i = 0; i < kSize; ++i, x /= 2)
		res[kSize - i - 1] += (x & 1);
	return res;
}

int main() {
	Trie<string, char> trie('*');
	trie.Add(To32String(0));

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
			auto num_str = trie.BiggestXORNum(x_str);
			auto num = stoi(num_str, nullptr, 2);
			cout << (num ^ x) << endl;
			break;
		}
	}

	return 0;
}