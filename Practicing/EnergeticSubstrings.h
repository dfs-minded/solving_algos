#include <iostream>
#include <algorithm>
#include <string>

using namespace std;

long long HeavyMetal(const string& s) {
	const string begin_str = "heavy";
	const string end_str = "metal";

	int ends_count = 0;
	for (auto i = s.find(end_str, 0); i != string::npos; 
		 i = s.find(end_str, i + end_str.size()))
		++ends_count;
	
	auto begin_index = s.find(begin_str, 0);
	auto end_index = s.find(end_str, 0);
	long long res = 0;

	while (begin_index != string::npos) {
		if (begin_index < end_index) {
			res += ends_count;
			begin_index = s.find(begin_str, begin_index + begin_str.size());
		}
		else {
			--ends_count;
			end_index = s.find(end_str, end_index + end_str.size());
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