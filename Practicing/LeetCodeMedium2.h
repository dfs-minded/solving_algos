#pragma once

bool canPartitionKSubsets(const vector<int>& nums, int k) {
	int sum = accumulate(nums.begin(), nums.end(), 0);
	if (sum % k != 0) return false;

	int target_sum = sum / k;

	map<int, bool> dp; // True, if the sum of numbers specified by dp is <= target_sum. False otherwise.
					// We add elements one by one and either get a new group or a number, which we can 
					// add to the growing group.
	dp[0] = true;
	map<int, int> rem_memo; // stores the remainders for the corresponding mask
	rem_memo[0] = 0;
	int last_mask = (1 << nums.size()) - 1;

	for (int mask = 0; mask <= last_mask; ++mask) {
		if (!dp[mask]) continue;

		for (int i = 0; i < nums.size(); ++i) {
			int rem = rem_memo[mask];
			int next_mask = mask | (1 << i);
			if (!(mask & (1 << i)) && // if the number not used
				dp.count(next_mask) == 0) { // if this mask was not previously calculated
				dp[next_mask] = (nums[i] + rem) <= target_sum;
				rem_memo[next_mask] = (nums[i] + rem) % target_sum;
			}
		}
	}

	return dp[last_mask];
}

vector<int> maxSumOfThreeSubarrays(vector<int>& nums, int k) {
	vector<int>  res;
	auto dp = vector(nums.size(), vector<int>(k)); // stores solution for the max sum we can get on the first i
				// positions in the array if the i-th element forms j-th group j=[0,k].

	return res;
}

int main() {

	/*vector<int> input{ 1,2,1,2,6,7,5,1 };
	auto res = maxSumOfThreeSubarrays(input, 2);*/

	int o; cin >> o;
	return 0;
}
