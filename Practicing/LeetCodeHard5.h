#pragma once

int subarraysWithKDistinct(vector<int>& A, int K) {
	const int kMaxVal = A.size() + 1;
	vector<int> vals_count(kMaxVal);

	int left{ 0 };
	int right{ 0 };
	int curr_distinct{ 0 };
	int good_subarrs_count{ 0 };

	while (right < A.size()) {
		while (right < A.size() && curr_distinct <= K) {
			if (vals_count[A[right]] == 0) ++curr_distinct;
			if (curr_distinct == K) ++good_subarrs_count;

			++vals_count[A[right]];
			++right;
		}

		while (curr_distinct >= K) {
			--vals_count[A[left]];
			if (vals_count[A[left]] == 0) --curr_distinct;
			if (curr_distinct == K) ++good_subarrs_count;
			++left;
		}

	}
	return good_subarrs_count;
}


int main() {
	vector<int> input{1,2,1,2,3};
	cout << subarraysWithKDistinct(input, 2);

	cin.get();
}