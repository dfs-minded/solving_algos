#pragma once

int wiggleMaxLengthOld(vector<int>& nums) {
	if (nums.size() <= 1) return 0;
	map<int, int> last_to_len_positive;
	map<int, int> last_to_len_negative;

	int diff = nums[1] - nums[0];
	if (diff > 0) last_to_len_positive[nums[1]] = 1;
	else last_to_len_negative[nums[1]] = 1;

	for (int i = 2; i < nums.size(); ++i) {
		auto iter = last_to_len_positive.lower_bound(nums[i]);
		if (iter != last_to_len_positive.begin()) {
			--iter;
			last_to_len_negative[nums[i]] = max(last_to_len_negative[nums[i]], iter->second + 1);
		}

		iter = last_to_len_negative.upper_bound(nums[i]);
		if (iter != last_to_len_negative.end()) {
			last_to_len_positive[nums[i]] = max(last_to_len_positive[nums[i]], iter->second + 1);
		}
	}

	auto cmp = [](const pair<int, int>& p1, const pair<int, int>& p2) {
		return p1.second < p2.second; };

	int res = max_element(last_to_len_positive.begin(), last_to_len_positive.end(), cmp)->second;
	res = max(res, max_element(last_to_len_negative.begin(), last_to_len_negative.end(), cmp)->second);
	return res;
}

int wiggleMaxLength(vector<int>& nums) {
	if (nums.empty()) return 0;
	vector<int> up(nums.size());
	vector<int> down(nums.size());

	for (int i = 1; i < nums.size(); ++i) {
		int max_up_len = 0;
		int max_down_len = 0;

		for (int j = 0; j < i; ++j) {
			if (nums[i] - nums[j] > 0)
				max_up_len = max(max_up_len, down[j] + 1);
			else if (nums[i] - nums[j] < 0)
				max_down_len = max(max_down_len, up[j] + 1);
		}

		up[i] = max_up_len;
		down[i] = max_down_len;
	}

	auto res = *max_element(up.begin(), up.end());
	res = max(res, *max_element(down.begin(), down.end()));
	return res;
}

int main() {
	vector<int> input = { 1,7,4,9,2,5 };
	cout << wiggleMaxLength(input);

	cin.get();
	return 0;
}