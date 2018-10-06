#include <iostream>
#include <algorithm>
#include <string>

using namespace std;

int HeavyMetal(const string& s) {
	const string begin_str = "heavy";
	const string end_str = "metal";

	auto begin_index = s.find(begin_str, 0);
	if (begin_index == string::npos) return 0;

	auto end_index = s.find(end_str, begin_index + begin_str.size());
	
	int begins_count = 1;
	int res = 1;

	while (end_index != string::npos) {
		if (begin_index != string::npos && begin_index < end_index) {
			begin_index = s.find(begin_str, begin_index + begin_str.size());
			if (begin_index != string::npos) {
				if (begin_index < end_index) ++res;
				++begins_count;
			}
		}
		else {
			end_index = s.find(end_str, end_index + end_str.size());
			if (end_index != string::npos) res += begins_count;
		}
	}

	return res;
}

int main() {
	string input;
	cin >> input;

	cout << HeavyMetal(input);

	//int o; cin >> o;
	return 0;
}