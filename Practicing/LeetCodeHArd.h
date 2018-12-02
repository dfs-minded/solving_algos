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

	int op = operators.top();
	operators.pop();

	switch (op) {
			case '*': a *= b;
			case '/': a /= b;
			case '+': a += b;
			case '-': a -= b;
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
			while (!operators.empty() && priority(s[i]) > priority(operators.top()))
				evaluate(operators, operands);
			operators.push(s[i]);
		}
		else if (isdigit(s[i])) {
			int x;
			//from_chars(&s[i], s.back(), x);
			operands.push(x);
			while (isdigit(s[i])) ++i;
			--i;
		}
	}

	while (!operators.empty()) evaluate(operators, operands);
	return operands.top();
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

vector<int> maxSumOfThreeSubarrays(vector<int>& nums, int m) {
	vector<int> sums_of_windows = ComputeSums(nums, m);

	vector<int> max_left_index(sums_of_windows.size());
	int max_sofar = sums_of_windows[0];
	for (int i = 1; i < sums_of_windows.size(); ++i) {
		max_left_index[i] = max_left_index[i - 1];
		if (sums_of_windows[i] > max_sofar) {
			max_left_index[i] = i;
			max_sofar = sums_of_windows[i];
		}
	}

	vector<int> max_right_index(sums_of_windows.size());
	max_right_index.back() = sums_of_windows.size() - 1;
	max_sofar = sums_of_windows.back();
	for (int i = sums_of_windows.size() - 2; i >= 0; --i) {
		max_right_index[i] = max_right_index[i + 1];
		if (sums_of_windows[i] > max_sofar) {
			max_right_index[i] = i;
			max_sofar = sums_of_windows[i];
		}
	}

	int j_res = -1; // start index of the middle window
	int curr_sum = numeric_limits<int>::min();
	for (int j = 1; j + 1 < sums_of_windows.size(); ++j) {
		int sum_on_lhs = sums_of_windows[max_left_index[j - 1]];
		int sum_on_rhs = sums_of_windows[max_right_index[j + 1]];
		if (sum_on_lhs + sums_of_windows[j] + sum_on_rhs > curr_sum)
			j_res = j;
	}

	return { max_left_index[j_res - 1], j_res, max_right_index[j_res + 1] };
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

	vector<int> input{ 1,2,1,2,6,7,5,1 };
	auto res = maxSumOfThreeSubarrays(input, 2);

	int o; cin >> o;
}