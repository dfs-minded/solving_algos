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


struct TreeNode {
	int val;
	TreeNode *left;
	TreeNode *right;
	TreeNode(int x) : val(x), left(NULL), right(NULL) {}
};

void levelOrderRec(TreeNode* node, int level, vector<vector<int>>& res) {
	if (!node) return;
	if (static_cast<int>(res.size() - 1) < level) 
		res.push_back(vector<int>());
	res[level].push_back(node->val);
	levelOrderRec(node->left, level + 1, res);
	levelOrderRec(node->right, level + 1, res);
}

vector<vector<int>> levelOrder(TreeNode* root) {
	vector<vector<int>> res;
	levelOrderRec(root, 0, res);
	return res;
}

int findLength(vector<int>& A, vector<int>& B) {
	vector<int> dp(A.size());
	int ans = 0;

	for (int i = 0; i < B.size(); ++i) {
		for (int j = A.size() - 1; j >= 0; --j) {
			int prev = j > 0 ? dp[j - 1] : 0;
			dp[j] = prev;
			if (A[i] == B[j]) ++dp[j];
			ans = max(ans, dp[j]);
		}
	}

	return ans;
}

int main() {

	/*vector<int> input{ 1,2,1,2,6,7,5,1 };
	auto res = maxSumOfThreeSubarrays(input, 2);*/

	/*vector<int> input = { 1,3,4,2,2 };
	cout << findDuplicate(input);*/

	TreeNode n1(3);
	TreeNode n2(9);
	TreeNode n3(20);
	TreeNode n4(15);
	TreeNode n5(7);

	n1.left = &n2;
	n1.right = &n3;
	n3.left = &n4;
	n3.right = &n5;


	auto res = levelOrder(&n1);
	int o; cin >> o;
	return 0;
}
