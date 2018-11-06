#include <iostream>
#include <fstream>
#include <vector>
#include <tuple>
#include <algorithm>
#include <functional>

using namespace std;

tuple<int, int, int> Read() {
	ifstream input;
	input.open("input.txt");

	int w, h, n;

	if (input.is_open()) {
		input >> w >> h >> n;
		input.close();
	}

	return { w, h, n };
}

void Write(int res) {
	ofstream output("output.txt");
	if (output.is_open())
	{
		output << res;
		output.close();
	}
}

bool CanPlace(int w, int h, long long n, int board_size) {
	long long hor_num = board_size / w;
	long long ver_num = board_size / h;

	return hor_num * ver_num >= n;
}

int Solve(int w, int h, int n) {
	int lo = 0;
	int hi = max(w, h) * n;

	while (hi - lo > 1) {
		int mid = lo + (hi - lo) / 2;
		if (CanPlace(w, h, n, mid)) hi = mid;
		else lo = mid;
	}

	return hi;
}


int main() {
	int w, h, n;

	tie(w, h, n) = Read();
	Write(Solve(w, h, n));

	return 0;
}

