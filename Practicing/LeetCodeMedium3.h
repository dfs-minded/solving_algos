#pragma once

int wiggleMaxLength(vector<int>& nums) {
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

int main() {
	vector<int> input = { 1,7,4,9,2,5 };
	cout << wiggleMaxLength(input);

	cin.get();
	return 0;
}