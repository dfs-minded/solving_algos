#pragma once

class NextPermutationSolution {
public:
	void nextPermutation(vector<int>& nums) {
		// find first smaller number to form inversion:
		int first_smaller_index = -1;
		for (int i = nums.size() - 2; i >= 0; --i) {
			if (nums[i] < nums[i + 1]) {
				first_smaller_index = i;
				break;
			}
		}

		if (first_smaller_index == -1) {
			reverse(nums.begin(), nums.end());
			return;
		}

		// Values on the right from first_smaller_index are in decreasing order. Find first from rhs which is greater:
		int last_bigger_index = nums.size() - 1;
		for (int j = nums.size() - 1; j > first_smaller_index; --j) {
			if (nums[j] > nums[first_smaller_index]) {
				last_bigger_index = j;
				break;
			}
		}

		swap(nums[first_smaller_index], nums[last_bigger_index]);
		// Since values on the right from first_smaller_index were in decreasing order, 
		// swap did not change the ordering, and next permutation has 
		reverse(nums.begin() + first_smaller_index + 1, nums.end());
	}
};


vector<vector<int>> threeSum(vector<int>& nums) {
	if (nums.size() < 3) return {};
	sort(nums.begin(), nums.end());

	vector<vector<int>> zero_sums;

	for (int left = 0; left < nums.size() - 2;) {
		int mid = left + 1;
		int right = nums.size() - 1;

		while (mid < right) {
			int curr_sum = nums[left] + nums[mid] + nums[right];
			if (curr_sum == 0)
				zero_sums.push_back({ nums[left], nums[mid], nums[right] });

			// move right pointer and skip duplicates for right number candidate:
			if (curr_sum > 0) 
				while (--right > mid && nums[right] == nums[right + 1]);
			else // move mid pointer and skip duplicates for mid number candidate:
				while (++mid < right && nums[mid] == nums[mid - 1]);
		}
		while (++left < nums.size() - 2 && nums[left] == nums[left - 1]);
	}	

	return zero_sums;
}

vector<vector<int>> threeSumV2(vector<int>& nums) {
	if (nums.size() < 3) return {};

	sort(nums.begin(), nums.end());
	nums.erase(unique(nums.begin(), nums.end()), nums.end());

	vector<vector<int>> zero_sums;

	map<int, set<int>> existing_nums_indices;
	for (int i = 0; i < nums.size(); ++i)
		existing_nums_indices[nums[i]].insert(i);

	for (int i = 1; i < nums.size(); ++i) {
		for (int j = 0; j < i; ++j) {
			int sum_left = -nums[i] - nums[j];
			if (existing_nums_indices.count(sum_left) > 0) {
				if (any_of(existing_nums_indices[sum_left].begin(), existing_nums_indices[sum_left].end(),
					[i, j](int index) { return index > i && index > j; }))
					zero_sums.push_back({ nums[i], nums[j], sum_left });
			}
		}
	}

	return zero_sums;
}

int longestConsecutive(vector<int>& nums) {
	unordered_set<int> elements;

	for (auto n : nums) elements.insert(n);

	int max_len_sofar = 0;

	for (auto n : elements) {
		// if this is not the smallest element in the sequence, continue
		if (elements.count(n - 1) > 0) continue;
		
		// while can continue existing bigger sequence:
		int curr_len = 1;
		while (elements.count(++n) > 0) ++curr_len;
		
		max_len_sofar = max(max_len_sofar, curr_len);
	}

	return max_len_sofar;
}

struct TreeNode {
	int val;
	TreeNode *left;
	TreeNode *right;
	TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};

int widthOfBinaryTree(TreeNode* root) {
	if (!root) return 0;
	stack<tuple<TreeNode*, int, int>> to_process; // DFS stack of a node, it's number and it's layer number
	to_process.push({ root, 1, 0 });
	unordered_map<int, pair<int, int>> layer_min_max;

	while (!to_process.empty()) {
		auto curr = to_process.top();
		to_process.pop();
		auto curr_layer = get<2>(curr);
		layer_min_max[curr_layer].first = min(layer_min_max[curr_layer].first, get<1>(curr));
		layer_min_max[curr_layer].second = max(layer_min_max[curr_layer].second, get<1>(curr));

		to_process.push({ get<0>(curr)->right, get<1>(curr) * 2 + 1, curr_layer + 1 });
		to_process.push({ get<0>(curr)->left, get<1>(curr) * 2, curr_layer + 1 });
	}

	int max_dist = 1;

	for (auto key_val : layer_min_max) {
		auto min_max = key_val.second;
		if (min_max.first != min_max.second)
			max_dist = max(max_dist, min_max.second - min_max.first + 1);
	}

	return max_dist;
}

int main() {
	/*NextPermutationSolution sln;
	vector<int> nums{2,3,1};
	sln.nextPermutation(nums);*/

	/*vector<int> input{ -1,0,1,2,-1,-4 };
	auto res = threeSumV2(input);*/

	vector<int> input{ 100,4,200,1,3,2 };
	int res = longestConsecutive(input);

	cin.get();
	return 0;
}