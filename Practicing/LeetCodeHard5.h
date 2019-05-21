#pragma once

int subarraysWithKDistinct(vector<int>& A, int K) {
	const int kMaxVal = A.size() + 1;
	vector<int> vals_count(kMaxVal);

	int left{ 0 };
	int right{ 0 };
	int curr_distinct{ 0 };
	int good_subarrs_count{ 0 };

	while (right < A.size()) {
		while (right < A.size() && curr_distinct <= K) {
			if (vals_count[A[right]] == 0) ++curr_distinct;
			if (curr_distinct == K) ++good_subarrs_count;

			++vals_count[A[right]];
			++right;
		}

		while (curr_distinct >= K) {
			--vals_count[A[left]];
			if (vals_count[A[left]] == 0) --curr_distinct;
			if (curr_distinct == K) ++good_subarrs_count;
			++left;
		}

	}
	return good_subarrs_count;
}


int findRotateSteps(string ring, string key) {
	map<char, vector<int>> char_to_pos_map;
	for (int i = 0; i < ring.size(); ++i)
		char_to_pos_map[ring[i]].push_back(i);

	vector<pair<int, int>> last_pos_and_steps, curr_pos_and_steps;
	last_pos_and_steps.push_back({ 0, 0 });
	int dial_size = ring.size();

	for (auto c : key) {
		for (auto curr_dial_pos : char_to_pos_map.at(c)) {
			int best_steps = numeric_limits<int>::max();
			for (const auto& pos_steps : last_pos_and_steps) {
				auto[pos, prev_steps] = pos_steps;
				//      <------> d1
				// ------------------------
				// <---> d2     <---------> d3    
				// curr_dist = min(d1, d2 + d3);
				int curr_dist = min(abs(pos - curr_dial_pos),
					min(pos, curr_dial_pos) + (dial_size - max(pos, curr_dial_pos)));
				best_steps = min(best_steps, prev_steps + curr_dist + 1/*for pushing a button*/);
			}
			curr_pos_and_steps.push_back({ curr_dial_pos, best_steps });
		}

		last_pos_and_steps = curr_pos_and_steps;
		curr_pos_and_steps = {};
	}

	return min_element(last_pos_and_steps.begin(), last_pos_and_steps.end(), 
		[](const pair<int, int>& p1, const pair<int, int>& p2) { return p1.second < p2.second; })->second;
}

class LargestIslandSolution {
public:
	int largestIsland(vector<vector<int>>& grid) {
		if (grid.empty()) return 0;
		int island_id = -1;
		unordered_map<int, int> island_id_to_size;

		for (int r = 0; r < grid.size(); ++r) {
			for (int c = 0; c < grid[0].size(); ++c) {
				if (grid[r][c] == 1) {
					island_id_to_size[island_id] = TraverseIsland(grid, r, c, island_id);
					--island_id;
				}
			}
		}

		return FindLargestMergedIsland(grid, island_id_to_size);
	}
private:
	array<int, 4> dx{ 0, 1, 0, -1 };
	array<int, 4> dy{ 1, 0, -1, 0 };

	int TraverseIsland(vector<vector<int>>& grid, int r, int c, int island_id) {
		queue<pair<int, int>> to_process;
		to_process.push({ r, c });
		grid[r][c] = island_id;
		int vertices_count = 0;
		while (!to_process.empty()) {
			++vertices_count;
			auto[curr_r, curr_c] = to_process.front();
			to_process.pop();
			
			for (int i = 0; i < dx.size(); ++i) {
				int next_r = curr_r + dy[i];
				int next_c = curr_c + dx[i];
				if (CanVisit(grid, next_r, next_c) && grid[next_r][next_c] == 1) {
					to_process.push({ next_r, next_c });
					grid[next_r][next_c] = island_id;
				}
			}
		}

		return vertices_count;
	}

	bool CanVisit(const vector<vector<int>>& grid, int r, int c) {
		if (r < 0 || c < 0 || r == grid.size() || c == grid[0].size()) return false;
		return true;
	}

	int FindLargestMergedIsland(const vector<vector<int>>& grid, const unordered_map<int, int> island_id_to_size) {
		int biggest_merged_island_size = 0;

		for (int r = 0; r < grid.size(); ++r) {
			for (int c = 0; c < grid[0].size(); ++c) {
				if (grid[r][c] != 0) continue;
				set<int> merged_islands;
				int curr_size = 0;

				for (int i = 0; i < dx.size(); ++i) {
					int next_r = r + dy[i];
					int next_c = c + dx[i];
					if (CanVisit(grid, next_r, next_c) && grid[next_r][next_c] < 0
						&& merged_islands.count(grid[next_r][next_c]) == 0) {
						curr_size += island_id_to_size.at(grid[next_r][next_c]);
						merged_islands.insert(grid[next_r][next_c]);
					}
				}
				biggest_merged_island_size = max(biggest_merged_island_size, curr_size);
			}
		}

		if (biggest_merged_island_size == 0) // all filled with '1' or all filled with '0'
			return island_id_to_size.empty() ? 1 : island_id_to_size.begin()->second;
		
		return biggest_merged_island_size + 1/*for flipped zero*/;
	}
};

int main() {
	//vector<int> input{1,2,1,2,3};
	//cout << subarraysWithKDistinct(input, 2);

	/*string ring = "godding";
	string key = "godding";
	cout << findRotateSteps(ring, key);*/

	LargestIslandSolution sln;
	vector<vector<int>> grid = { {1, 1}, {1, 1} };
	cout << sln.largestIsland(grid);

	cin.get();
}