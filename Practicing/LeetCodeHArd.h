#pragma once

// Returns a vector of indices of first smaller element on the left of the given element.
template<class InputIt>
vector<int> ComputeFirstSmaller(InputIt first, InputIt last) { // TODO this does not work properly
	vector<int> indices(abs(std::distance(first, last)));
	stack<pair<int, int>> smaller; // pair of val to index
	smaller.push({ -1,  0});
	
	int i = 0;
	while (first != last) {
		while (smaller.top().first >= *first) smaller.pop();
		indices[i] = smaller.top().second;
		
		smaller.push({ *first, i + 1});
		++i;
		++first;
	}

	return indices;
}

vector<int> ComputeFirstSmallerOnTheLeft(const vector<int>& heights) {
	int N = heights.size();
	vector<int> indices(N);
	stack<pair<int, int>> smaller; // pair of val to index
	smaller.push({ -1,  0 });

	for (int i = 0; i < N; ++i) {
		while (smaller.top().first >= heights[i]) smaller.pop();
		indices[i] = smaller.top().second;

		smaller.push({ heights[i], i + 1 });
	}

	return indices;
}

vector<int> ComputeFirstSmallerOnTheRight(const vector<int>& heights) {
	int N = heights.size();
	vector<int> indices(N);
	stack<pair<int, int>> smaller; // pair of val to index
	smaller.push({ -1,  N });

	for (int i = N - 1; i >= 0; --i) {
		while (smaller.top().first >= heights[i]) smaller.pop();
		indices[i] = smaller.top().second;

		smaller.push({ heights[i], i });
	}

	return indices;
}

int MaxAreaHistograms(const vector<int>& heights) {
	if (heights.empty()) return 0;
	auto first_smaller_left_indx = ComputeFirstSmallerOnTheLeft(heights);
	auto first_smaller_right_indx = ComputeFirstSmallerOnTheRight(heights);

	int max_area = heights[0];
	for (int i = 0; i < heights.size(); ++i) {
		int curr_area = (first_smaller_right_indx[i] - first_smaller_left_indx[i]) * heights[i];
		if (curr_area > max_area) max_area = curr_area;
	}

	return max_area;
}


// Returns a matrix, whire each value is the number of 
// consequative ones (added from left to right) between zoroes.
vector<vector<int>> CalcCountMatrix(const vector<vector<char>>& input) {
	int R = input.size();
	int C = input[0].size();
	vector<vector<int>> count(R, vector<int>(C));

	for (int r = 0; r < R; ++r) {
		int curr_sum = 0;
		for (int c = 0; c < C; ++c) {
			if (input[r][c] == '1') ++curr_sum;
			else curr_sum = 0;
			count[r][c] = curr_sum;
		}
	}

	return count;
}

// Returns nearest smaller lenght of ones on the top for each run of ones in the count matrix
vector<vector<int>> CalcSmallerUpMatrix(const vector<vector<int>>& count) {
	int R = count.size();
	int C = count[0].size();
	vector<vector<int>> smaller_up(R, vector<int>(C));

	for (int c = 0; c < C; ++c) {
		stack<pair<int, int>> increasing; // pair of number to row index
		for (int r = 0; r < R; ++r) {
			while (!increasing.empty() && increasing.top().first >= count[r][c])
				increasing.pop();
			if (!increasing.empty()) smaller_up[r][c] = increasing.top().second;
			else smaller_up[r][c] = -1;

			increasing.push({ count[r][c], r });
		}
	}

	return smaller_up;
}

// Returns nearest smaller lenght of ones on the bottom for each run of ones in the count matrix
vector<vector<int>> CalcSmallerDownMatrix(const vector<vector<int>>& count) {
	int R = count.size();
	int C = count[0].size();
	vector<vector<int>> smaller_down(R, vector<int>(C));

	for (int c = 0; c < C; ++c) {
		stack<pair<int, int>> increasing; // pair of number to row index
		for (int r = R - 1; r >= 0; --r) {
			while (!increasing.empty() && increasing.top().first >= count[r][c])
				increasing.pop();
			if (!increasing.empty()) smaller_down[r][c] = increasing.top().second;
			else smaller_down[r][c] = R;

			increasing.push({ count[r][c], r });
		}
	}

	return smaller_down;
}

int CalcMaxArea(const vector<vector<int>>& count,
	const vector<vector<int>>& smaller_up, const vector<vector<int>>& smaller_down) {
	int R = count.size();
	int C = count[0].size();

	int max_area_sofar = 0;
	for (int r = 0; r < R; ++r) {
		for (int c = 0; c < C; ++c) {
			int height = smaller_down[r][c] - smaller_up[r][c] - 1;
			max_area_sofar = max(max_area_sofar, count[r][c] * height);
		}
	}

	return max_area_sofar;
}

int maximalRectangle(vector<vector<char>>& matrix) {
	int R = matrix.size();
	if (R == 0) return 0;
	int C = matrix[0].size();

	auto count = CalcCountMatrix(matrix);
	auto smaller_up = CalcSmallerUpMatrix(count);
	auto smaller_down = CalcSmallerDownMatrix(count);

	int res = CalcMaxArea(count, smaller_up, smaller_down);
	return res;
}

int maximalRectangle2(vector<vector<char>>& matrix) {
	int R = matrix.size();
	if (R == 0) return 0;
	int C = matrix[0].size();

	vector<int> width(R);
	int max_area = 0;

	for (int c = 0; c < C; ++c) {
		// update width
		for (int r = 0; r < R; ++r) {
			width[r] += 1;
			width[r] *= (matrix[r][c] == '0' ? 0 : 1);
		}

		vector<int> nearest_smaller_up(R, -1);
		vector<int> nearest_smaller_down(R, R);

		// pairs of value to index
		stack<pair<int, int>> increasing_stack_up;
		stack<pair<int, int>> increasing_stack_down;

		for (int r = 0; r < R; ++r) {
			// calc nearest smaller up
			while (!increasing_stack_up.empty() && increasing_stack_up.top().first >= width[r])
				increasing_stack_up.pop();

			if (!increasing_stack_up.empty())
				nearest_smaller_up[r] = increasing_stack_up.top().second;

			increasing_stack_up.push({ width[r], r });

			// calc nearest smaller down
			while (!increasing_stack_down.empty() && increasing_stack_down.top().first >= width[R - r - 1])
				increasing_stack_down.pop();

			if (!increasing_stack_down.empty())
				nearest_smaller_down[R - r - 1] = increasing_stack_down.top().second;

			increasing_stack_down.push({ width[R - r - 1], R - r - 1 });
		}

		// calc current areas and update max area
		for (int r = 0; r < R; ++r) {
			int height = nearest_smaller_down[r] - nearest_smaller_up[r] - 1;
			max_area = max(max_area, width[r] * height);
		}
	}

	return max_area;
}


using int64 = long long;
int64 Split(const vector<int>& nums, int64 max_sum) {
	int segments = 1;
	int64 curr_sum = 0;

	for (int i = 0; i < nums.size(); ++i) {
		if (nums[i] > max_sum) return numeric_limits<long long>::max();
		if (curr_sum + nums[i] <= max_sum) curr_sum += nums[i];
		else {
			curr_sum = nums[i];
			++segments;
		}
	}

	return segments;
}
int splitArray(vector<int>& nums, int m) {
	int64 lo = 0;
	int64 hi = accumulate(nums.begin(), nums.end(), 0ll);

	while (hi - lo > 1) {
		int64 mid = lo + (hi - lo) / 2;
		if (Split(nums, mid) > m) lo = mid; // need to increase max_sum
		else hi = mid;
	}

	return hi;
}

int longestConsecutive(vector<int>& nums) {
	unordered_map<int, int> vals_to_used;

	for (auto v : nums) vals_to_used[v] = 1;

	int res = 1;
	for (int i = 0; i < nums.size(); ++i) {
		int length = 1;

		int l = nums[i] - 1;
		while (vals_to_used.count(l) > 0 && vals_to_used[l] == 1) {
			++length;
			vals_to_used[l] = 0;
			--l;
		}

		int r = nums[i] + 1;
		while (vals_to_used.count(r) > 0 && vals_to_used[r] == 1) {
			++length;
			vals_to_used[r] = 0;
			++r;
		}

		res = max(res, length);
	}

	return res;
}

bool isOperator(char c) {
	set<char> operators{ '+', '-', '*', '/' };
	return operators.count(c) > 0;
}

bool isDelim(char c) {
	return c == ' ';
}

int priority(char c) {
	switch(c) {
			case '(': return 3;
            case '*':
			case '/': return 2;
			case '+':
			case '-': return 1;
			default: return -1;
	}
}

void evaluate(stack<char>& operators, stack<int>& operands) {
	int b = operands.top();
	operands.pop();
	int a = operands.top(); // store final result in a
	operands.pop();

	char op = operators.top();
	operators.pop();

	switch (op) {
		case '*': a *= b; break;
		case '/': a /= b; break;
		case '+': a += b; break;
		case '-': a -= b; break;
	}

	operands.push(a);
}

#include <charconv>
int calculate(string s) {
	stack<char> operators;
	stack<int> operands;

	for (int i = 0; i < s.size(); ++i) {
		if (isDelim(s[i])) continue;

		if (s[i] == '(') operators.push(s[i]);
		else if (s[i] == ')') {
			while (operators.top() != '(') evaluate(operators, operands);
			operators.pop();
		}
		else if (isOperator(s[i])) {
			while (!operators.empty() && priority(s[i]) >= priority(operators.top()))
				evaluate(operators, operands);
			operators.push(s[i]);
		}
		else if (isdigit(s[i])) {
			int x;
			//from_chars(&s[i], &s.back(), x);
			x = atoi(s.substr(i).c_str());
			operands.push(x);
			while (isdigit(s[i])) ++i;
			--i;
		}
	}

	while (!operators.empty()) evaluate(operators, operands);
	return operands.empty() ? 0 : operands.top();
}


vector<int> ComputeSums(vector<int>& nums, int m) {
	vector<int> res;
	int sum = 0;
	for (int i = 0; i < nums.size(); ++i) {
		if (i < m) sum += nums[i];
		else {
			res.push_back(sum);
			sum -= nums[i - m];
			sum += nums[i];
		}
	}
	res.push_back(sum);
	return res;
}



class Solution {
public:
	int swimInWater(const vector<vector<int>>& grid) {
		int R = grid.size();
		int C = grid[0].size();

		priority_queue<Site, vector<Site>, greater<Site>> to_process;

		for (int r = 0; r < R; ++r)
			for (int c = 0; c < C; ++c)
				to_process.push(Site(r, c, grid[r][c]));

		int last_vertex_id = R * C - 1;
		UF union_find(last_vertex_id);
		int res = -1;

		while (!union_find.IsConnected(0, last_vertex_id)) {
			auto curr = to_process.top();
			to_process.pop();
			res = curr.Val;
			int curr_point_uf_coord = ToUFCoordinates(curr.R, curr.C, R);
			if (curr.R > 0 && grid[curr.R - 1][curr.C] <= res) // top
				union_find.Connect(curr_point_uf_coord, ToUFCoordinates(curr.R - 1, curr.C, R));

			if (curr.C < grid[0].size() - 1 && grid[curr.R][curr.C + 1] <= res) // right
				union_find.Connect(curr_point_uf_coord, ToUFCoordinates(curr.R, curr.C + 1, R));

			if (curr.R < grid.size() - 1 && grid[curr.R + 1][curr.C] <= res) // down
				union_find.Connect(curr_point_uf_coord, ToUFCoordinates(curr.R + 1, curr.C, R));

			if (curr.C > 0 && grid[curr.R][curr.C - 1] <= res) // left
				union_find.Connect(curr_point_uf_coord, ToUFCoordinates(curr.R, curr.C - 1, R));
		}

		return res;
	}
private:

	struct Site {
		int R;
		int C;
		int Val;

		Site(int r, int c, int val) : R(r), C(c), Val(val) {};

		bool operator > (const Site& other) const {
			return Val > other.Val;
		}

	};

	int ToUFCoordinates(int r, int c, int row_size) {
		return r * row_size + c;
	}

	class UF {
	public:
		UF(int N) { data.resize(N + 1, -1); }

		void Connect(int p, int q) {
			if (IsConnected(p, q)) return;
			int r1 = Root(p);
			int r2 = Root(q);
			if (r1 > r2) {
				data[r1] += data[r2];
				data[r2] = r1;
			}
			else {
				data[r2] += data[r1];
				data[r1] = r2;
			}
		}

		bool IsConnected(int p, int q) {
			int r1 = Root(p);
			int r2 = Root(q);
			return r1 == r2;
		}

	private:
		vector<int> data;

		int Root(int p) {
			while (data[p] > 0) p = data[p];
			return p;
		}
	};
};


vector<int> maxSumOfThreeSubarrays(vector<int>& nums, int k) {
	const int kSubArrsNum = 3;
	// stores max sum we can get for each number of sub-arrays (<=3 for this task) ending at index i
	vector<int> prev_dp(nums.size()), dp(nums.size()); 

	vector<int> prefix_sums(nums.size() + 1);
	for (int i = 1; i < prefix_sums.size(); ++i)
		prefix_sums[i] += prefix_sums[i - 1] + nums[i - 1];

	vector<vector<int>> best_last_taken_index(kSubArrsNum, vector<int>(dp.size(), -1));

	for (int m = 0; m < kSubArrsNum; ++m) {
		for (int i = (m + 1) * k - 1; i < dp.size(); ++i) {
			dp[i] = i - 1 > 0 ? dp[i - 1] : 0;
			best_last_taken_index[m][i] = i > 0 ? best_last_taken_index[m][i - 1] : -1;
			
			int prev_last_index = i - (m + 1) * k;
			int curr = prefix_sums[i + 1];
			if (prev_last_index >= 0) {
				curr += prev_dp[prev_last_index] - prefix_sums[prev_last_index + 1];
			}
			if (curr > dp[i]) {
				dp[i] = curr;
				best_last_taken_index[m][i] = prev_last_index - k + 1;
			}
		}
		swap(prev_dp, dp);
	}

	int max_sum_sofar = -1;
	int best_index = -1;
	for (int i = 0; i < dp.size(); ++i) {
		if (max_sum_sofar < dp[i]) {
			max_sum_sofar = dp[i];
			best_index = i;
		}
	}

	vector<int> res(kSubArrsNum);
	for (int m = kSubArrsNum - 1; m > 0; --m) {
		res[m] = best_index;
		best_index = best_last_taken_index[m][best_index];
	}
	res[0] = best_index;

	return res;
}

int main() {
	/*vector<int> input{ 2,1,5,6,2,3 };
	cout << MaxAreaHistograms(input);*/

	/*vector<vector<char>> matrix = {
		{ '1','0','1','0','0' },
		{ '1','0','1','1','1' },
		{ '1','1','1','1','1' },
		{ '1','0','0','1','0' }
	};

	vector<vector<char>> matrix1 = { 
		{ '0','0','1','0' },
		{ '1','1','1','1' },
		{ '1','1','1','1' },
		{ '1','1','1','0' },
		{ '1','1','0','0' },
		{ '1','1','1','1' },
		{ '1','1','1','0' } 
	};

	vector<vector<char>> matrix2 = { { '1' } };

	vector<vector<char>> matrix3 = {
		{ '1','0','1','1','1'},
		{ '0','1','0','1','0' },
		{ '1','1','0','1','1' },
		{ '1','1','0','1','1' },
		{ '0','1','1','1','1' }
	};

	cout << maximalRectangle2(matrix3);*/

	/*vector<int> input{ 10, 2, 3 };
	cout << splitArray(input, 2);*/

	/*vector<int> input{ 100, 4, 200, 1, 3, 2 };
	cout << longestConsecutive(input);*/

	/*string input = "(1+(4+5+2)-3)+(6+8)";
	string input2 = " 2-1 + 2 ";
	cout << calculate(input);*/

	/*Solution s;
	vector<vector<int>> input = { {6,23,16,13,7},{8,19,14,2,18},{0,22,17,21,9},{12,1,10,24,4},{5,20,3,11,15} };
	cout << s.swimInWater(input);*/

	vector<int> input = { 1,2,1,2,6,7,5,1 };
	auto res = maxSumOfThreeSubarrays(input, 2);

	int o; cin >> o;
}