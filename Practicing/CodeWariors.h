#pragma once

int SimpleReversalGame(int n, int k) {
	vector<int> res(n, 0);
	int d = 0;
	for (int i = 0; i < n; ++i) {
		if (i % 2) {
			res[d] = i;
			++d;
		}
		else {
			res[n - d - 1] = i;
		}
	}

	return res[k];
}