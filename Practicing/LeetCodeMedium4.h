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


/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode(int x) : val(x), left(NULL), right(NULL) {}
 * };
 */
class Solution {
public:
	TreeNode* lowestCommonAncestor(TreeNode* root, TreeNode* p, TreeNode* q) {
		int nodes_found = 0;
		return LCA(root, p, q, nodes_found);
	}
private:
	TreeNode* LCA(TreeNode* curr, TreeNode* p, TreeNode* q, int& num_nodes_found) {
		if (!curr) return nullptr;

		int nodes_found_left = 0;
		auto* left = LCA(curr->left, p, q, nodes_found_left);
		if (nodes_found_left == 2) {
			num_nodes_found = 2;
			return left;
		}

		int nodes_found_right = 0;
		auto* right = LCA(curr->right, p, q, nodes_found_right);
		if (nodes_found_right == 2) {
			num_nodes_found = 2;
			return right;
		}

		num_nodes_found = nodes_found_left + nodes_found_right;
		if (curr->val == p->val || curr->val == q->val) ++num_nodes_found;

		if (num_nodes_found == 2) return curr;

		return nullptr;
	}
};
 
class LCASolution {
public:
	TreeNode* lowestCommonAncestor(TreeNode* root, TreeNode* p, TreeNode* q) {
		bool p_or_q_found = false;
		return LCA(root, p, q, p_or_q_found);
	}
private:
	TreeNode* LCA(TreeNode* node, TreeNode* p, TreeNode* q, bool& p_or_q_found) {
		if (!node) return nullptr;

		bool p_or_q_found_left = false;
		auto* left = LCA(node->left, p, q, p_or_q_found_left);
		if (left) return left;

		bool p_or_q_found_right = false;
		auto* right = LCA(node->right, p, q, p_or_q_found_right);
		if (right) return right;

		if (node == p || node == q) p_or_q_found = true;

		if ((p_or_q_found_left && p_or_q_found_right) ||
			(p_or_q_found && (p_or_q_found_left || p_or_q_found_right)))
			return node; // node is lca


		p_or_q_found = p_or_q_found || p_or_q_found_left || p_or_q_found_right;
		return nullptr;
	}
};

int compress(vector<char>& chars) {
	int j = 0;
	int count = 0;
	char last = chars[0];

	for (int i = 0; i < chars.size(); ++i) {
		if (chars[i] == last) {
			++count;
			continue;
		}

		chars[j++] = last;
		last = chars[i];
		if (count > 1) {
			auto count_str = to_string(count);
			for (auto c : count_str) chars[j++] = c;
		}
		count = 1;
	}
	
	chars[j++] = last;
	if (count > 1) {
		auto count_str = to_string(count);
		for (auto c : count_str) chars[j++] = c;
	}

	return j;
}

int canCompleteCircuit(vector<int>& gas, vector<int>& cost) {
	int N = gas.size();
	int start = 0;
	int end = 0;
	int curr_tank = gas[start];

	while (start < N) {

		while (curr_tank >= cost[end]) {
			curr_tank -= cost[end];

			end = (end + 1) % N;
			curr_tank += gas[end];

			if (end == start) return start;
		}

		if (end != start) {
			curr_tank -= gas[start];
			curr_tank += cost[start];
			++start;
		}
		else {
			++start;
			end = start;
			curr_tank = gas[start];
		}
	}

	return -1;
}

class WordSearchSolution {
public:
	bool exist(vector<vector<char>>& board, string word) {
		for (int r = 0; r < board.size(); ++r)
			for (int c = 0; c < board[0].size(); ++c) {
				if (board[r][c] != word[0]) continue;
				board[r][c] = kUsedMarker;

				if (SearchWordDfs(board, { r, c }, word, 1))
					return true;

				board[r][c] = word[0];
			}

		return false;
	}
private:
	const vector<int> kDx = { 0, 1, 0, -1 };
	const vector<int> kDy = { 1, 0, -1, 0 };
	const char kUsedMarker = '*';

	bool SearchWordDfs(vector<vector<char>>& board, pair<int, int> pos,
		 const string& word, int d) {
		if (d == word.length()) return true;
		auto[r, c] = pos;

		for (int i = 0; i < kDx.size(); ++i) {
			int next_r = r + kDy[i];
			int next_c = c + kDx[i];

			if (next_r < 0 || next_r == board.size() ||
				next_c < 0 || next_c == board[0].size()) continue;

			if (board[next_r][next_c] == word[d]) {
				board[next_r][next_c] = kUsedMarker;

				if (SearchWordDfs(board, { next_r, next_c }, word, d + 1))
					return true;

				board[next_r][next_c] = word[d];
			}
		}

		return false;
	}
};

class LifeSolution {
public:
	void gameOfLife(vector<vector<int>>& board) {
		if (board.empty()) return;

		constexpr array<int, 8> dx{ 0, 1, 1, 1, 0, -1, -1, -1 };
		constexpr array<int, 8> dy{ 1, 1, 0, -1, -1, -1, 0, 1 };
		constexpr int kWasOneMarker = 2; // and became 0
		constexpr int kWasZeroMarker = 3; // and became 1

		for (int r = 0; r < board.size(); ++r) {
			for (int c = 0; c < board[0].size(); ++c) {
				int live_adj = 0;

				for (int i = 0; i < dx.size(); ++i) {
					int adj_r = r + dy[i];
					int adj_c = c + dx[i];
					if (adj_r < 0 || adj_r == board.size() || adj_c < 0 || adj_c == board[0].size())
						continue;
					if (board[adj_r][adj_c] == 1 || board[adj_r][adj_c] == kWasOneMarker)
						++live_adj;
				}

				if (live_adj < 2 || live_adj > 3)
					board[r][c] = board[r][c] == 1 ? kWasOneMarker : 0;
				else if (live_adj == 3)
					board[r][c] = board[r][c] == 0 ? kWasZeroMarker : 1;
			}
		}

		for (int r = 0; r < board.size(); ++r) {
			for (int c = 0; c < board[0].size(); ++c) {
				if (board[r][c] == kWasOneMarker) board[r][c] = 0;
				else if (board[r][c] == kWasZeroMarker) board[r][c] = 1;
			}
		}
	}
};

int main() {
	/*NextPermutationSolution sln;
	vector<int> nums{2,3,1};
	sln.nextPermutation(nums);*/

	/*vector<int> input{ -1,0,1,2,-1,-4 };
	auto res = threeSumV2(input);*/

	/*vector<int> input{ 100,4,200,1,3,2 };
	int res = longestConsecutive(input);*/

	/*LCASolution sln;
	auto* n0 = new TreeNode(0);
	auto* n1 = new TreeNode(1);
	auto* n2 = new TreeNode(2);
	auto* n3 = new TreeNode(3);
	auto* n4 = new TreeNode(4);
	auto* n5 = new TreeNode(5);
	auto* n6 = new TreeNode(6);
	auto* n7 = new TreeNode(7);
	auto* n8 = new TreeNode(8);

	n3->left = n5;
	n5->left = n6;
	n5->right = n2;
	n2->left = n7;
	n2->right = n4;

	cout << sln.lowestCommonAncestor(n3, n5, n4)->val;*/

	//vector<char> input { 'a','b','c' };
	/*vector<char> input { 'a','a','b','b','c','c','c' };
	auto res = compress(input);*/
	/*vector<int> gas{ 1,2,3,4,5 };
	vector<int> cost{3,4,5,1,2};
	cout << canCompleteCircuit(gas, cost);*/

	/*vector<vector<char>> board{ {'A', 'B', 'C', 'E'}, {'S', 'F', 'C', 'S'}, {'A', 'D', 'E', 'E'} };
	WordSearchSolution sln;
	cout << sln.exist(board, "ABCCED");*/

	LifeSolution sln;
	vector<vector<int>> board{ {0,1,0},{0,0,1},{1,1,1},{0,0,0} };
	sln.gameOfLife(board);

	cin.get();
	return 0;
}