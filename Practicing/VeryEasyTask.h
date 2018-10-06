#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int CalcMinTime(int n, int x, int y) {
	if (n == 0) return 0;
	if (n == 1) return min(x, y);

	--n; // make first copy on faster printer
	int lo = 0;
	int hi = n * max(x, y);
	auto pred = [n, x, y] (int t) { // returns true if can make enough copies
		return (t/x + t/y) >= n;
	};

	while (hi - lo > 1) {
		int mid = lo + (hi - lo) / 2;
		if (pred(mid)) hi = mid;
		else lo = mid;
	}

	return hi + min(x, y);
}

int main() {
	int n, x, y;
	cin >> n >> x >> y;

	cout << CalcMinTime(n, x, y);

	//int o; cin >> o;
	return 0;
}

