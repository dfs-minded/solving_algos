#pragma once

class RemoveDuplicateLettersSolution {
public:
	string removeDuplicateLetters(const string& s) {
		fill_n(last_index_.begin(), kRadix, s.size());
		fill_n(used_.begin(), kRadix, false);
		all_occurences_ = vector<vector<int>>(kRadix, vector<int>());

		for (int i = 0; i < s.size(); ++i) {
			all_occurences_[s[i] - 'a'].push_back(i);
			last_index_[s[i] - 'a'] = i;
		}

		int chars_left = 0;
		for (int i = 0; i < kRadix; ++i) {
			if (last_index_[i] != s.size()) ++chars_left;
			else used_[i] = true;
		}

		string res = "";

		while (chars_left) {
			for (int i = 0; i < kRadix; ++i) {
				if (used_[i]) continue;

				for (auto pos : all_occurences_[i]) {
					if (CanTakeAsNext(pos)) {
						res += ('a' + i);
						used_[i] = true;
						--chars_left;
						break;
					}
				}
				if (used_[i]) break;
			}
		}

		return res;
	}

private:
	static constexpr int kRadix = 26;

	array<int, kRadix> last_index_;
	vector<vector<int>> all_occurences_;
	array<bool, kRadix> used_;
	mutable int min_index_{ -1 };

	bool CanTakeAsNext(int pos) const {
		if (pos < min_index_) return false;

		for (int i = 0; i < kRadix; ++i) {
			if (!used_[i] && last_index_[i] < pos)
				return false;
		}

		min_index_ = pos;
		return true;
	}
};


static constexpr int pow(int x, int p) noexcept {
	int res = 1;
	int base = x;

	while (p) {
		if (p & 1) res *= base;
		base *= base;
		p /= 2;
	}

	return res;
}

class SmallestRangeSolution {
public:
	vector<int> smallestRange(vector<vector<int>>& nums) {
		constexpr int kMaxValAbs{ 2 * pow(10, 5) };

		vector<set<int>> values_present_to_lists_ids(kMaxValAbs + 1);
		int ListsNum = nums.size();

		for (int k = 0; k < ListsNum; ++k) {
			for (auto num : nums[k])
				values_present_to_lists_ids[num + kMaxValAbs / 2].insert(k);
		}

		constexpr int kMaxListsNum{ 3500 };
		vector<int> lists_has_val_in_range(kMaxListsNum);

		int curr_lists_represented = 0;

		int best_lo = 0, best_hi = kMaxValAbs;
		int lo = 0, hi = 0;

		while (hi <= kMaxValAbs) {
			while (hi <= kMaxValAbs && curr_lists_represented < ListsNum) {
				for (auto list_id : values_present_to_lists_ids[hi]) {
					if (lists_has_val_in_range[list_id] == 0)
						++curr_lists_represented;
					++lists_has_val_in_range[list_id];
				}
				++hi;
			}
			--hi;

			if (curr_lists_represented == ListsNum && (best_hi - best_lo > hi - lo)) {
				best_lo = lo;
				best_hi = hi;
			}

			while (curr_lists_represented == ListsNum) {
				for (auto list_id : values_present_to_lists_ids[lo]) {
					--lists_has_val_in_range[list_id];
					if (lists_has_val_in_range[list_id] == 0)
						--curr_lists_represented;
				}
				++lo;
			}
			--lo;
		}

		return { best_lo - kMaxValAbs / 2, best_hi - kMaxValAbs / 2 };
	}
};


class Solution {
public:
	int numBusesToDestination(vector<vector<int>>& routes, int S, int T) {
		stop_id_to_rout_id_ = vector<vector<int>>(Solution::kMaxStopId);
		auto graph = BuildGraph(routes);
		return SearchTargetStop(graph, S, T);
	}

private:
	static constexpr int kMaxStopId{ pow(10, 6) };
	static constexpr int kMaxRouts{ 500 };

	vector<vector<int>> stop_id_to_rout_id_;

	vector<vector<int>> BuildGraph(const vector<vector<int>>& routes) {

		for (int rout_id = 0; rout_id < routes.size(); ++rout_id) {
			for (auto stop_id : routes[rout_id])
				stop_id_to_rout_id_[stop_id].push_back(rout_id);
		}

		vector<vector<int>> graph(kMaxRouts);

		for (auto& rout_ids : stop_id_to_rout_id_) {
			for (int i = 1; i < rout_ids.size(); ++i) {
				for (int j = 0; j < i; ++j) {
					graph[rout_ids[i]].push_back(rout_ids[j]);
					graph[rout_ids[j]].push_back(rout_ids[i]);
				}
			}
		}

		return graph;
	}

	int SearchTargetStop(const vector<vector<int>>& graph, int S, int T) {
		queue<pair<int, int>> to_process;
		vector<int> visited(Solution::kMaxRouts);

		for (auto route_id : stop_id_to_rout_id_[S]) { // add all routs containing start stop
			if (find(stop_id_to_rout_id_[T].begin(), stop_id_to_rout_id_[T].end(), route_id) != 
					stop_id_to_rout_id_[T].end())
				return (S == T) ? 0 : 1;
			to_process.push({ route_id, 1 });
		}

		while (!to_process.empty()) {
			auto[curr_rout_id, dist] = to_process.front();
			to_process.pop();
			visited[curr_rout_id] = true;

			for (auto adj_rout_id : graph[curr_rout_id]) {
				if (visited[adj_rout_id]) continue;
				if (find(stop_id_to_rout_id_[T].begin(), stop_id_to_rout_id_[T].end(), adj_rout_id) !=
						stop_id_to_rout_id_[T].end())
					return dist + 1;

				to_process.push({ adj_rout_id, dist + 1 });
			}
		}

		return -1;
	}
};

class SlidingPuzzleSolution {
public:
	int slidingPuzzle(const vector<vector<int>>& board) {
		map<vector<vector<int>>, int> min_steps_dp;
		vector<vector<int>> target_board{ {1, 2, 3}, {4, 5, 0} };
		min_steps_dp[target_board] = 0;

		queue<vector<vector<int>>> to_process;
		to_process.push(target_board);
		set<vector<vector<int>>> used;
		used.insert(target_board);

		const vector<int> dc{ 0, 1, 0, -1 };
		const vector<int> dr{-1, 0, 1, 0 };

		while (!to_process.empty()) {
			auto curr_board = to_process.front();
			to_process.pop();

			auto[zero_r, zero_c] = GetZeroPos(board);

			for (int i = 0; i < dc.size(); ++i) {
				auto next_r = zero_r + dr[i];
				auto next_c = zero_c + dc[i];
				if (next_r >= 0 && next_r < board.size() && next_c >= 0 && next_c < board[0].size()) {
					auto next_board = curr_board;
					swap(next_board[zero_r][zero_c], next_board[next_r][next_c]);
					if (used.count(next_board)) continue;
					used.insert(next_board);
					to_process.push(next_board);

					if (min_steps_dp.count(next_board) == 0)
						min_steps_dp[next_board] = min_steps_dp[curr_board] + 1;
					else 
						min_steps_dp[next_board] = min(min_steps_dp[next_board], min_steps_dp[curr_board] + 1);
				}
			}
		}
		return (min_steps_dp.count(board) == 0) ? -1 : min_steps_dp[board];
	}
private:
	pair<int, int> GetZeroPos(const vector<vector<int>>& board) {
		for (int r = 0; r < board.size(); ++r) {
			for (int c = 0; c < board[0].size(); ++c)
				if (board[r][c] == 0) {
					return { r, c };
				}
		}

		return { -1, -1 };
	}
};


int lengthOfLongestSubstringKDistinct(const string& s, int k) {
	if (s.empty() || k == 0) return 0;
	constexpr int kBase{ 256 };
	array<int, kBase> count;
	fill_n(count.begin(), kBase, 0);

	int start{ 0 };
	int longest_sofar{ 0 };
	int distinct{ 0 };

	for (int end = 0; end < s.size();) {
		while (end < s.size() && (distinct < k || count[s[end]] > 0)) {
			if (count[s[end]] == 0) ++distinct;
			++count[s[end]];
			++end;
		}

		longest_sofar = max(longest_sofar, end - start);

		while (distinct == k) {
			--count[s[start]];
			if (count[s[start]] == 0) --distinct;
			++start;
		}
	}

	return longest_sofar;
}


int main() {
	//RemoveDuplicateLettersSolution sln;
	//sln.removeDuplicateLetters("yioccqiorhtoslwlvfgzycahonecugtatbyphpuunwvaalcpndabyldkdtzfjlgwqk");
	
	//vector<vector<int>> input = { {4,10,15,24,26},{0,9,12,20},{5,18,22,30} };
	//SmallestRangeSolution sln;
	//auto res = sln.smallestRange(input);
	
	//Solution s;
	//vector<vector<int>> input{ {7,12},{4,5,15},{6},{15,19},{9,12,13} };
	//auto res = s.numBusesToDestination(input, 15, 12);

	/*SlidingPuzzleSolution sln;
	vector<vector<int>> input{ {4, 1, 2},{5,0,3} };
	cout << sln.slidingPuzzle(input);*/

	cout << lengthOfLongestSubstringKDistinct("aba", 1);

	cin.get();
}
