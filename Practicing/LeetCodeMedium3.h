#pragma once

using namespace std;


int wiggleMaxLength(vector<int>& nums) {
	if (nums.size() <= 1) return nums.size();
	int res = 1;

	int i = 1;
	int diff = nums[i] - nums[i - 1];
	while (i < nums.size() && diff == 0) {
		++i;
		diff = nums[i] - nums[i - 1];
	}


	if (i == nums.size()) return res;
	++res;
	while (i < nums.size()) {
		diff = nums[i] - nums[i - 1];

		// seach for next critical point
		if (diff >= 0)
			while (i < nums.size() && nums[i] >= nums[i - 1]) ++i;
		else
			while (i < nums.size() && nums[i] <= nums[i - 1]) ++i;

		if (i == nums.size()) return res;

		// next critical point found
		++res;
	}

	return res;
}

int wiggleMaxLengthBetter(vector<int>& nums) {
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



int maxProfitLinearMemory(const vector<int>& prices) {
	if (prices.empty()) return 0;
	vector<int> open_dp(prices.size());
	open_dp[0] = -prices[0];
	vector<int> closed_dp(prices.size());

	for (int i = 1; i < prices.size(); ++i) {
		closed_dp[i] = max(closed_dp[i - 1], open_dp[i - 1] + prices[i]);
		open_dp[i] = max(open_dp[i - 1], closed_dp[i - 2] - prices[i]);
	}

	return closed_dp.back();
}

int maxProfit(const vector<int>& prices) {
	if (prices.empty()) return 0;
	int best_profit_open = -prices[0];
	int best_profit_closed_2_days_ago = 0;
	int best_profit_closed = 0;

	for (int i = 1; i < prices.size(); ++i) {
		auto best_closed_day_ago = best_profit_closed;
		best_profit_closed = max(best_closed_day_ago, best_profit_open + prices[i]);
		best_profit_open = max(best_profit_open, best_profit_closed_2_days_ago - prices[i]);
		best_profit_closed_2_days_ago = best_closed_day_ago;
	}

	return best_profit_closed;
}


int findTargetSumWays(vector<int>& nums, int S) {
	int total_sum = accumulate(nums.begin(), nums.end(), 0);
	if (total_sum < abs(S)) return 0;

	// total_sum will correspond to zero in dp array, so we can safely add and substract nums[i]
	int max_sum_possible = 2 * total_sum;
	vector<int> target_sum_ways_dp_prev(max_sum_possible + 1);
	vector<int> target_sum_ways_dp_curr(max_sum_possible + 1);
	target_sum_ways_dp_prev[total_sum + nums[0]] = 1;
	target_sum_ways_dp_prev[total_sum - nums[0]] += 1;

	for (int i = 1; i < nums.size(); ++i) {
		for (int s = 0; s <= max_sum_possible; ++s) {
			target_sum_ways_dp_curr[s] = 0;
			if (s - nums[i] >= 0)
				target_sum_ways_dp_curr[s] += target_sum_ways_dp_prev[s - nums[i]]; // take with positive sgn
			if (s + nums[i] <= max_sum_possible)
				target_sum_ways_dp_curr[s] += target_sum_ways_dp_prev[s + nums[i]]; // take with negative sgn
		}

		swap(target_sum_ways_dp_curr, target_sum_ways_dp_prev);
	}

	return target_sum_ways_dp_prev[total_sum + S];
}


int numTrees(int n) {
	if (!n) return 0;
	vector<int> catalan_numbers(n + 1);
	catalan_numbers[0] = 1;
	catalan_numbers[1] = 1;
        
	for (int i = 2; i <= n; ++i) {
		int ith_catalan_num = 0;
		for (int j = 0; j < i; ++j)
			ith_catalan_num += catalan_numbers[j]*catalan_numbers[i - j - 1];
		catalan_numbers[i] = ith_catalan_num;
	}
        
	return catalan_numbers.back();
}

int leastInterval(vector<char>& tasks, int n) {
	const int kAlphabetRadix = 26;
	int total_intervals_needed = tasks.size();

	vector<int> letters_count(kAlphabetRadix);
	for (auto t : tasks) ++letters_count[t - 'A'];

	sort(letters_count.rbegin(), letters_count.rend());

	int tasks_left_to_process = tasks.size();
	vector<bool> used(kAlphabetRadix);

	while (tasks_left_to_process) {
		int time_left_till_same_task = n + 1;
		
		while (time_left_till_same_task) {
			int next_task_index = -1;
			int next_task_count = 0;
			for (int i = 0; i < kAlphabetRadix; ++i) {
				if (!used[i] && letters_count[i] > next_task_count) {
					next_task_count = letters_count[i];
					next_task_index = i;
				}
			}

			if (next_task_index == -1) break;

			used[next_task_index] = true;

			--letters_count[next_task_index];
			--tasks_left_to_process;
			--time_left_till_same_task;
		}

		if (tasks_left_to_process)
			total_intervals_needed += time_left_till_same_task;

		fill(used.begin(), used.end(), false);
	}

	return  total_intervals_needed;
}

int leastIntervalOld(vector<char>& tasks, int n) {
	unordered_map<char, int> tasks_count;
	for (auto t : tasks) tasks_count[t]++;

	multiset<pair<int, char>> tasks_to_process;
	for (auto pair : tasks_count)
		tasks_to_process.insert({ pair.second, pair.first });

	int total_intervals_needed = tasks.size();
	auto pos = tasks_to_process.begin();
	int intervals_left = n + 1;
	char first_task = pos->second;
	while (!tasks_to_process.empty()) {
		--intervals_left;
		char curr_task = pos->second;
		int new_val = pos->first - 1;

		tasks_to_process.erase(pos);
		pos = tasks_to_process.insert({ new_val, curr_task });

		if (pos->first == 0)
			pos = tasks_to_process.erase(pos);
		else ++pos;

		if (tasks_to_process.empty()) break;

		if (pos == tasks_to_process.end())
			pos = tasks_to_process.begin();

		if (intervals_left == 0) {
			first_task = pos->second;
			intervals_left = n + 1;
		} else if (pos->second == first_task) {
			total_intervals_needed += intervals_left;
			intervals_left = n + 1;
		}
	}

	return  total_intervals_needed;
}


vector<vector<string>> groupAnagrams(const vector<string>& input) {
	const int kAlphabetRadix = 26;
	unordered_map<string, vector<string>> letters_count_to_words;

	for (auto word : input) {
		vector<int> alphabet_count(kAlphabetRadix);
		for (auto letter : word)
			++alphabet_count[letter - 'a'];
		string alphabet_count_str(alphabet_count.begin(), alphabet_count.end());
		letters_count_to_words[alphabet_count_str].push_back(word);
	}

	// vector<vector<string>> res;
	// transform(letters_count_to_words.begin(), letters_count_to_words.end(), back_inserter(res), 
	//          [] (auto& map_pair) { return map_pair.second; });
	vector<vector<string>> res(letters_count_to_words.size());
	int i = 0;
	for (auto& key_val : letters_count_to_words)
		res[i++] = key_val.second;

	return res;
}


bool canPartitionKSubsets(const vector<int> nums, int k) {
	long long total_sum = accumulate(nums.begin(), nums.end(), 0ll);
	if (total_sum % k != 0) return false;
	int target_sum = total_sum / k;

	int last_mask = pow(2, nums.size()) - 1;
	vector<int> mask_to_rem_dp(last_mask + 1, -1);
	mask_to_rem_dp[0] = 0;

	for (int mask = 0; mask <= last_mask; ++mask) {
		if (mask_to_rem_dp[mask] == -1) continue;

		for (int i = 0; i < nums.size(); ++i) {
			int curr_mask = mask | (1 << i);
			if (mask_to_rem_dp[curr_mask] != -1) continue;

			int curr_sum = mask_to_rem_dp[mask] + nums[i];
			
			if (curr_sum <= target_sum) {
				int remainder = curr_sum % target_sum;
				mask_to_rem_dp[curr_mask] = remainder;
			}
		}
	}

	return mask_to_rem_dp[last_mask] == 0;
}


class NumIslandsSolution {
public:
	int numIslands(vector<vector<char>>& grid) {
		int num_of_islands = 0;
		if (grid.empty()) return num_of_islands;

		for (int r = 0; r < grid.size(); ++r) {
			for (int c = 0; c < grid[0].size(); ++c) {
				if (grid[r][c] == kIslandMarker) {
					++num_of_islands;
					TraverseIsland(r, c, grid);
				}
			}
		}

		return num_of_islands;
	}

private:
	const char kNoIslandMarker{ '0' };
	const char kIslandMarker{ '1' };
	const char kVisitedMarker{ '2' };


	void TraverseIsland(int r, int c, vector<vector<char>>& grid) {
		queue<pair<int, int>> to_traverse;
		to_traverse.push({ r, c });

		while (!to_traverse.empty()) {
			tie(r, c) = to_traverse.front();
			to_traverse.pop();

			if (grid[r][c] != kIslandMarker) continue;

			grid[r][c] = kVisitedMarker;

			if (r > 0) to_traverse.push({ r - 1, c }); // top

			if (c < grid[0].size() - 1) to_traverse.push({ r, c + 1 }); // right

			if (r < grid.size() - 1) to_traverse.push({ r + 1, c }); // down

			if (c > 0) to_traverse.push({ r, c - 1 }); // left
		}
	}
};



struct ListNode {
	int val;
	ListNode *next;
    ListNode(int x) : val(x), next(NULL) {}
};
 
class SortListSolution {
public:
	ListNode* sortList(ListNode* head) {
		int nodes_count = NodesCount(head);

		for (int merge_run_len = 1; merge_run_len < nodes_count; merge_run_len *= 2) {
			auto* first = head;
			auto* second = head;
			ListNode* last_merged_end = nullptr;

			while (first) {
				auto* first_run_end = AdvancePointer(first, merge_run_len);
				second = first_run_end ? first_run_end->next : nullptr;
				if (first_run_end)
					first_run_end->next = nullptr; // disconnect first run

				auto* second_run_end = AdvancePointer(second, merge_run_len);
				auto* after_second_run = second_run_end ? second_run_end->next : nullptr;
				if (second_run_end)
					second_run_end->next = nullptr; // disconnect second run

				auto merged_begin_end = Merge(first, second);

				if (last_merged_end)
					last_merged_end->next = merged_begin_end.first;
				else head = merged_begin_end.first;

				last_merged_end = merged_begin_end.second;
				last_merged_end->next = after_second_run;

				first = last_merged_end->next;
			}
		}
		return head;
	}
private:
	// Returns first and last nodes in a merged list
	pair<ListNode*, ListNode*> Merge(ListNode* first, ListNode* second) {
		ListNode* merged_head;
		if (first && !second || (first && first->val < second->val)) {
			merged_head = first;
			first = first->next;
		}
		else {
			merged_head = second;
			second = second->next;
		}

		ListNode* merged = merged_head;
		while (first || second) {
			if (first && !second || (first && first->val < second->val)) {
				merged->next = first;
				merged = merged->next;
				first = first->next;
			}
			else {
				merged->next = second;
				merged = merged->next;
				second = second->next;
			}
		}
		return { merged_head, merged };
	}

	ListNode* AdvancePointer(ListNode* node, int val) {
		while (--val && node)
			node = node->next;
		return node;
	}

	int NodesCount(ListNode* node) {
		int nodes_number = 0;
		while (node) {
			++nodes_number;
			node = node->next;
		}
		return nodes_number;
	}

	ListNode* sortListN2(ListNode* head) {
		int nodes_number = NodesCount(head);

		for (int i = nodes_number - 1; i >= 0; --i) {
			auto* tmp = head;
			for (int j = 0; j < i; ++j) {
				if (tmp->next->val < tmp->val)
					swap(tmp->next->val, tmp->val);
				tmp = tmp->next;
			}
		}

		return head;
	}
};


class MaximalSquareSolution {
public:
	int maximalSquare(vector<vector<char>>& matrix) {
		if (matrix.empty()) return 0;

		// max square side length, which has its bottom right corner at [i][j]:
		vector<int> max_square_side_dp_prev(matrix[0].size());
		auto max_square_side_dp_curr = max_square_side_dp_prev;
		int max_side_sofar = 0;
		vector<int> vertical_ones_num(matrix[0].size());

		for (int r = 0; r < matrix.size(); ++r) {
			int horizontal_ones_num = 0;
			for (int c = 0; c < matrix[0].size(); ++c) {
				if (matrix[r][c] - '0') {
					int prev_square_side = 0;
					if (c != 0)
						prev_square_side = max_square_side_dp_prev[c - 1];

					max_square_side_dp_curr[c] = min({ vertical_ones_num[c], horizontal_ones_num, prev_square_side }) + 1;

					++horizontal_ones_num;
					++vertical_ones_num[c];
				} else {
					max_square_side_dp_curr[c] = 0;
					horizontal_ones_num = 0;
					vertical_ones_num[c] = 0;
				}

				max_side_sofar = max(max_side_sofar, max_square_side_dp_curr[c]);
			}

			swap(max_square_side_dp_curr, max_square_side_dp_prev);
		}
	
		return max_side_sofar * max_side_sofar;
	}
};


bool exist(vector<vector<char>>& board, string word) {
	if (word.empty()) return true;

	// make exhaustive search on start of the word through the whole board
	for (int r = 0; r < board.size(); ++r) {
		for (int c = 0; c < board[0].size(); ++c) {
			if (board[r][c] != word[0]) continue;
			
			// Use DFS for the word lookup
			vector<vector<bool>> used(board.size(), vector<bool>(board[0].size()));
			stack<tuple<int, int, int, bool>> to_process; // tuple of row col, chars matched and visited flag
			to_process.push({ r, c, 1, false });

			while (!to_process.empty()) {
				int r, c, num_matched, visited;
				tie(r, c, num_matched, visited) = to_process.top();
				if (visited) {
					to_process.pop();
					used[r][c] = false;
					continue;
				}

				if (num_matched == word.size()) return true;

				used[r][c] = true;
				get<3>(to_process.top()) = true;
				char curr_char = word[num_matched];

				if (r > 0 && !used[r - 1][c] && board[r - 1][c] == curr_char) // top
					to_process.push({ r - 1, c, num_matched + 1, false });
				if (c < board[0].size() - 1 && !used[r][c + 1] && board[r][c + 1] == curr_char) // right
					to_process.push({ r, c + 1, num_matched + 1, false });
				if (r < board.size() - 1 && !used[r + 1][c] && board[r + 1][c] == curr_char) // down
					to_process.push({ r + 1, c, num_matched + 1, false });
				if (c > 0 && !used[r][c - 1] && board[r][c - 1] == curr_char) // left
					to_process.push({ r, c - 1, num_matched  + 1, false });
			}
		}
	}

	return false;
}


class NextPermutationSolution {
public:
	void nextPermutation(vector<int>& nums) {
		if (nums.size() <= 1) return;

		// find first smaller number to form inversion:
		int last_bigger_index = nums.size() - 1;
		int first_smaller_index = -1;

		for (int i = nums.size() - 2; i >= 0; --i) {
			if (nums[i] < nums[last_bigger_index]) {
				first_smaller_index = i;
				break;
			}
			else last_bigger_index = i;
		}

		if (first_smaller_index == -1) {
			sort(nums.begin(), nums.end());
			return;
		}

		// from the biggest values on the right, find the smallest one
		last_bigger_index = first_smaller_index + 1;
		for (int j = first_smaller_index + 2; j < nums.size(); ++j) {
			if (nums[j] <= nums[last_bigger_index])
				last_bigger_index = j;
		}

		swap(nums[first_smaller_index], nums[last_bigger_index]);
		insertionSort(nums, first_smaller_index + 1, nums.size());
	}
private:
	void insertionSort(vector<int>& nums, int begin, int end) {
		for (int i = begin; i < end; ++i)
			for (int j = i; j > begin; --j)
				if (nums[j] < nums[j - 1]) swap(nums[j], nums[j - 1]);
	}
};

int main() {
	/*vector<int> input = { 1,7,4,9,2,5 };
	cout << wiggleMaxLength(input);*/

	/*string input = "2[2[y]p1[u]]ef";
	string input2 = "100[leetcode]";
	int last_parsed = 0;
	cout << solveDecode(input, last_parsed);*/

	/*vector<int> input = { 0, 0, 0, 0, 0, 0, 0, 0, 1 };
	cout << findTargetSumWays(input, 1);*/
	
	/*[0, 0, 0, 0, 0, 0, 0, 0, 1]
	1*/

	/*vector<char> input{ 'A','A','A','B','B','B' };
	cout << leastInterval(input, 2);*/

	//vector<string> input{"eat", "tea", "tan", "ate", "nat", "bat"};
	//auto res = groupAnagrams(input);

	//vector<int> input{ 2,2,2,2,3,4,5 }; // ->false
	//cout << canPartitionKSubsets(input, 4); 

	//vector<vector<char>> input =
	//	{ {'1','1','0','0','0'},{'1','1','0','0','0'},{'0','0','1','0','0'},{'0','0','0','1','1'} };

	//NumIslandsSolution s;
	//cout << s.numIslands(input);

	//SortListSolution sls;
	//// [-1,5,3,4,0]
	//auto* l1 = new ListNode(-1);
	//auto* l2 = new ListNode(5);
	//auto* l3 = new ListNode(3);
	//auto* l4 = new ListNode(4);
	//auto* l5 = new ListNode(0);

	//l1->next = l2;
	//l2->next = l3;
	//l3->next = l4;
	//l4->next = l5;

	//sls.sortList(l1);

	//MaximalSquareSolution sln;
	//vector<vector<char>> matrix { {'1','0','1','0','0'},{'1','0','1','1','1'},{'1','1','1','1','1'},{'1','0','0','1','0'} };
	//vector<vector<char>> matrix2 { {'1','0','1','1','0','1'},
	//{'1','1','1','1','1','1'},
	//{'0','1','1','0','1','1'},
	//{'1','1','1','0','1','0'},
	//{'0','1','1','1','1','1'},
	//{'1','1','0','1','1','1'}}; // 4

	//vector<vector<char>> matrix3{	{ '0','0','0','1' },
	//								{ '1','1','0','1' },
	//								{ '1','1','1','1' },
	//								{ '0','1','1','1' },
	//								{ '0','1','1','1' } }; // 9
	//vector<vector<char>> matrix4 { {'1', '1', '1', '1', '1', '1', '1', '1'},
	//								{ '1','1','1','1','1','1','1','0' },
	//								{ '1','1','1','1','1','1','1','0' },
	//								{ '1','1','1','1','1','0','0','0' },
	//								{ '0','1','1','1','1','0','0','0' } }; // 16
	//
	//cout << sln.maximalSquare(matrix2);

	vector<vector<char>> matrix{{ 'A','B','C','E' }, 
								{ 'S','F','C','S' }, 
								{ 'A','D','E','E' } };
	string word = "ABCB";

	cout << exist(matrix, word);

	cin.get();
	return 0;
}