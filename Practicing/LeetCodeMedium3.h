#pragma once

using namespace std;

int wiggleMaxLength(vector<int>& nums) {
	if (nums.empty()) return 0;
	vector<int> up(nums.size());
	vector<int> down(nums.size());

	stack<int> nearest_lesser_indx;
	stack<int> nearest_greater_indx;

	for (int i = 0; i < nums.size(); ++i) {
		while (!nearest_lesser_indx.empty() && nums[i] <= nums[nearest_lesser_indx.top()])
			nearest_lesser_indx.pop();
		if (!nearest_lesser_indx.empty()) // update |down| sequence
			down[i] = up[nearest_lesser_indx.top()] + 1;
		nearest_lesser_indx.push(i);

		while (!nearest_greater_indx.empty() && nums[i] >= nums[nearest_greater_indx.top()])
			nearest_greater_indx.pop();
		if (!nearest_greater_indx.empty()) // update |up| sequence
			up[i] = down[nearest_greater_indx.top()] + 1;
		nearest_greater_indx.push(i);

		if (i > 0) {
			down[i] = max(down[i], down[i - 1]);
			up[i] = max(up[i], up[i - 1]);
		}
	}

	return max(down.back(), up.back()) + 1;
}

int wiggleMaxLengthSlow(vector<int>& nums) {
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

string solveDecode(const string& s, int& i) {
	string res = "";
	
	while (i < s.size() && s[i] != ']') {
		if (!isdigit(s[i])) res += s[i++];
		else {
			int count = 0;
			while (isdigit(s[i]))
				count = count * 10 + (s[i++] - '0');

			++i; // '['
			auto sub_expression = solveDecode(s, i);
			++i; // ']'

			res.reserve(res.size() + count * sub_expression.size());
			while (count--) 
				res += sub_expression;
		}
	}
	return res;
}

int findTargetSumWays(vector<int>& nums, int S) {
	int sum = accumulate(nums.begin(), nums.end(), 0);
	if (sum < S) return 0;

	vector<int> dp(sum + 1);
	dp[0] = 1;

	for (int i = 0; i < nums.size(); ++i) {
		for (int s = sum - nums[i]; sum >= nums[i]; --s) {
			dp[s] += dp[s - nums[i]]; // take with positive sgn
			dp[s] += dp[s + nums[i]]; // take with negative sgn
		}
	}

	return dp[S];
}

int main() {
	/*vector<int> input = { 1,7,4,9,2,5 };
	cout << wiggleMaxLength(input);*/

	string input = "2[2[y]p1[u]]ef";
	string input2 = "100[leetcode]";
	int last_parsed = 0;
	cout << solveDecode(input, last_parsed);

	/*vector<int> input = { 1, 1, 1, 1, 1 };
	findTargetSumWays(input, 3);*/
	
	cin.get();
	return 0;
}