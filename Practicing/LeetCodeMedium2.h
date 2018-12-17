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


int findLengthDP(vector<int>& A, vector<int>& B) {
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
// Solution with hash
using ull = unsigned long long;

bool EqualSubArrExists(vector<int>& A, vector<int>& B, int length) {
	unordered_set<ull> first_str_hashes;
	const ull kP = 179;
	ull kP_n = 1;
	for (int i = 0; i < length - 1; ++i)
		kP_n *= kP;

	// precompute all hashes of subarrays of length |length| for |A| using rolling hash algo:
	ull h1 = 0;
	for (int i = 0; i < length; ++i)
		h1 = h1 * kP + A[i];

	first_str_hashes.insert(h1);

	for (int i = length; i < A.size(); ++i) {
		h1 = (h1 - A[i - length] * kP_n) * kP + A[i];
		first_str_hashes.insert(h1);
	}

	// traverse all hashes in |B| and see if there is a matching hash in |first_str_hashes|:
	ull h2 = 0;
	for (int i = 0; i < length; ++i)
		h2 = h2 * kP + B[i];
	if (first_str_hashes.count(h2) > 0) return true;

	for (int i = length; i < B.size(); ++i) {
		h2 = (h2 - B[i - length] * kP_n) * kP + B[i];
		if (first_str_hashes.count(h2) > 0) return true;
	}

	return false;
}

int findLength(vector<int>& A, vector<int>& B) {
	int lo = 0;
	int hi = min(A.size(), B.size());
	if (EqualSubArrExists(A, B, hi)) return hi;

	// invariant: lo - sub arr exists of length lo, hi - does not exist
	while (hi - lo > 1) {
		int mid = lo + (hi - lo) / 2;
		if (EqualSubArrExists(A, B, mid)) lo = mid;
		else hi = mid;
	}

	return lo;
}

void rotate(vector<vector<int>>& matrix) {
	int N = matrix.size();
	for (int r = 0; r < N / 2; ++r) {
		for (int c = 0; c < (N + 1) / 2; ++c) {
			int val = matrix[r][c];
			for (int i = 0; i < 4; ++i) {
				int new_r = c;
				int new_c = N - r - 1;
				int tmp = matrix[new_r][new_c];
				matrix[new_r][new_c] = val;
				r = new_r;
				c = new_c;
				val = tmp;
			}
		}
	}
}


void solveCombinationSum(const vector<int>& candidates, int pos, int target,
	vector<int>& curr, vector<vector<int>>& res) {
	if (pos == candidates.size()) {
		if (target == 0) res.push_back(curr);
		return;
	}

	// skip current number
	solveCombinationSum(candidates, pos + 1, target, curr, res);

	// include curr number
	for (int s = candidates[pos]; s <= target; s += candidates[pos]) {
		curr.push_back(candidates[pos]);
		solveCombinationSum(candidates, pos + 1, target - s, curr, res);
	}

	while (!curr.empty() && curr.back() == candidates[pos])
		curr.pop_back();
}

vector<vector<int>> combinationSum(vector<int>& candidates, int target) {
	if (candidates.empty()) return {};

	vector<int> curr;
	vector<vector<int>> res;

	solveCombinationSum(candidates, 0, target, curr, res);

	return res;
}

class MaxProfitSolution {
public:
	int maxProfit(const vector<int>& prices) {
		memo = vector<int>(prices.size(), -1);
		return solveMaxProfit(prices, 0);
	}
private:
	vector<int> memo;

	int solveMaxProfit(const vector<int>& prices, int pos) {
		if (pos >= prices.size() - 1) return 0;
		if (memo[pos] != -1) return memo[pos];

		int res = solveMaxProfit(prices, pos + 1); // do not sell

		for (int i = pos + 1; i < prices.size(); ++i) {
			res = max(res,
				(prices[i] - prices[pos]) + solveMaxProfit(prices, i + 2)); // sell at i-th day 
		}

		memo[pos] = res;
		return res;
	}
};

bool searchMatrix(vector<vector<int>>& matrix, int target) {
	if (matrix.empty() || matrix[0].empty()) 
		return false;
	if (target < matrix.front().front() || target > matrix.back().back()) 
		return false;

	int R = matrix.size();
	int C = matrix[0].size();
	int lo = 0;
	int hi = R * C - 1;

	while (hi - lo > 1) {
		int mid = lo + (hi - lo) / 2;
		int r = mid / C;
		int c = mid % C;
		if (matrix[r][c] < target) lo = mid;
		else if (matrix[r][c] > target) hi = mid;
		else return true;
	}

	return (matrix[lo / C][lo % C] == target) || 
		   (matrix[hi / C][hi % C] == target);
}

int main() {

	/*vector<int> input{ 1,2,1,2,6,7,5,1 };
	auto res = maxSumOfThreeSubarrays(input, 2);*/

	/*vector<int> input = { 1,3,4,2,2 };
	cout << findDuplicate(input);*/

	//TreeNode n1(3);
	//TreeNode n2(9);
	//TreeNode n3(20);
	//TreeNode n4(15);
	//TreeNode n5(7);

	//n1.left = &n2;
	//n1.right = &n3;
	//n3.left = &n4;
	//n3.right = &n5;


	//auto res = levelOrder(&n1);
	/*vector<int> A = { 1, 2, 3, 2, 1 };
	vector<int> B = { 3, 2, 1, 4, 7 };*/
	/*vector<int> A = { 0, 0, 0, 0, 0 };
	vector<int> B = { 0, 0, 0, 0, 0 };*/
	/*vector<int> A = { 0, 0, 0, 0, 0, 0, 1, 0, 0, 0 };
	vector<int> B = { 0, 0, 0, 0, 0, 0, 0, 1, 0, 0 };*/

	//ut << findLength(A, B);

	/*vector<vector<int>> input = {
		{1, 2, 3},
		{4, 5, 6 },
		{7, 8, 9},
	};

	rotate(input);*/
	/*vector<int> input = { 2,3,6,7 };
	auto res = combinationSum(input, 7);*/

	/*MaxProfitSolution sln;
	vector<int> input = { 1,2,3,0,2 };
	cout << sln.maxProfit(input);*/

	/*vector<vector<int>> input = { {} };
	cout << searchMatrix(input, 0);*/

	cin.get();
	return 0;
}
