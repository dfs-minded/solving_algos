#pragma once

int trap(vector<int>& height) {
	if (height.empty()) return 0;

	// store the nearest left bar of greater height in the stack:
	stack<pair<int, int>> height_to_start_index;
	height_to_start_index.push({ height[0], 0 });

	int water_sofar = 0;

	for (int i = 1; i < height.size(); ++i) {
		// subtract accumulated by smaller elevations water (we want to "delete" them completely):
		while (height_to_start_index.size() > 1 && height_to_start_index.top().first < height[i]) {
			auto prev = height_to_start_index.top();
			height_to_start_index.pop();
			auto prev_prev = height_to_start_index.top();

			water_sofar = water_sofar - (prev.second - prev_prev.second - 1) * prev.first
							- prev.first; // subtract elevation of the prev bar itself
		}

		// add water, which we can store between current and previous bar, as if there were no bars in between
		// (we have "deleted" them on the previous step)
		auto prev = height_to_start_index.top();
		water_sofar += (i - prev.second - 1) * min(prev.first, height[i]);

		if (height_to_start_index.top().first < height[i])
			height_to_start_index.pop();

		height_to_start_index.push({ height[i], i });
	}

	return water_sofar;
}

int maximalRectangle(vector<vector<char>>& matrix) {
	if (matrix.empty()) return 0;
	int R = matrix.size();
	int C = matrix[0].size();

	vector<vector<int>> aux_matrix(R, vector<int>(C)); // sums of consecutive '1's counted from left to right 

	// index of the nearest smaller element in |aux_matrix|
	auto first_smaller_up = aux_matrix;
	auto first_smaller_down = aux_matrix;

	int max_area = 0;

	// fill auxiliary matrix:
	for (int r = 0; r < R; ++r) {
		for (int c = 0; c < C; ++c) {
			int prev = c > 0 ? aux_matrix[r][c - 1] : 0;
			int curr = matrix[r][c] == '0' ? 0 : 1;
			aux_matrix[r][c] = (prev + curr) * curr;
		}
	}

	// fill first smaller up and down matrices:
	for (int c = 0; c < C; ++c) {
		stack<int> smaller_up_index;
		stack<int> smaller_down_index;

		for (int r = 0; r < R; ++r) {

			while (!smaller_up_index.empty() && aux_matrix[smaller_up_index.top()][c] >= aux_matrix[r][c])
				smaller_up_index.pop();

			first_smaller_up[r][c] = smaller_up_index.empty() ? -1 : smaller_up_index.top();
			smaller_up_index.push(r);

			while (!smaller_down_index.empty() && aux_matrix[smaller_down_index.top()][c] >= aux_matrix[R - r - 1][c])
				smaller_down_index.pop();

			first_smaller_down[R - r - 1][c] = smaller_down_index.empty() ? R : smaller_down_index.top();
			smaller_down_index.push(R - r - 1);
		}
	}

	// calc max area:
	for (int r = 0; r < R; ++r) {
		for (int c = 0; c < C; ++c) {
			int curr_area = (first_smaller_down[r][c] - first_smaller_up[r][c] - 1) * aux_matrix[r][c];
			max_area = max(max_area, curr_area);
		}
	}

	return max_area;
}

int largestRectangleArea(vector<int>& heights) {
	vector<int> nearest_smaller_left(heights.size()); // indices
	auto nearest_smaller_right = nearest_smaller_left;

	stack<int> min_stack;
	for (int i = 0; i < heights.size(); ++i) {
		while (!min_stack.empty() && heights[min_stack.top()] >= heights[i])
			min_stack.pop();

		nearest_smaller_left[i] = min_stack.empty() ? -1 : min_stack.top();
		min_stack.push(i);
	}
	min_stack = {};
	for (int i = heights.size() - 1; i >= 0; --i) {
		while (!min_stack.empty() && heights[min_stack.top()] >= heights[i])
			min_stack.pop();

		nearest_smaller_right[i] = min_stack.empty() ? heights.size() : min_stack.top();
		min_stack.push(i);
	}

	int max_area = 0;
	for (int i = 0; i < heights.size(); ++i) {
		int width = nearest_smaller_right[i] - nearest_smaller_left[i] - 1;
		max_area = max(max_area, heights[i] * width);
	}

	return max_area;
}

string minWindow(string s, string t) {
	const int kBase = 256;
	vector<int> count(kBase); // target chars count

	int chars_left = 0; // number of characters left to match
	for (auto c : t) {
		if (count[c] == 0) ++chars_left;
		++count[c];
	}

	vector<int> curr_count(kBase);
	int best_left = -1, best_right = s.size();

	for (int left = 0, right = 0; right < s.size();) {
		while (chars_left && right < s.size()) {
			int c = s[right++];
			++curr_count[c];
			if (count[c] == curr_count[c]) --chars_left;
		}

		while (left < right) {
			int c = s[left];
			if (count[c] == curr_count[c]) break; // do not remove chars, that we need
			--curr_count[c];
			++left;
		}

		if (chars_left == 0 && (right - left) < (best_right - best_left)) {
			best_left = left;
			best_right = right;
		}

		// remove one needed char
		--curr_count[s[left]];
		++chars_left;
		++left;
	}

	return (best_left == -1) ? "" : s.substr(best_left, best_right - best_left);
}

double findMedianSortedArrays(vector<int>& nums1, vector<int>& nums2) {
	if (nums1.empty() || nums2.empty()) {
		if (nums1.empty()) swap(nums1, nums2);
		int median_index = (nums1.size() - 1) / 2;
		return (nums1.size() % 2 == 0) ? (nums1[median_index] + nums1[median_index + 1]) / 2.0 :
			nums1[median_index];
	}

	// search for position of elements from the second array in the first one.
	if (nums2.back() > nums1.back()) swap(nums1, nums2);

	auto merged_size = nums1.size() + nums2.size();
	auto median_index = (merged_size - 1) / 2;

	int lo = 0, hi = nums2.size() - 1;
	while (lo <= hi) {
		int mid = lo + (hi - lo) / 2;
		auto iters_pair = equal_range(nums1.begin(), nums1.end(), nums2[mid]);
		auto insertion_pos_lo = distance(nums1.begin(), iters_pair.first);
		auto merged_index_lo = mid + insertion_pos_lo;

		if (merged_index_lo > median_index) {
			hi = mid - 1;
			continue;
		}

		auto insertion_pos_hi = distance(nums1.begin(), iters_pair.second);
		auto merged_index_hi = mid + insertion_pos_hi;

		if (merged_index_hi < median_index) {
			lo = mid + 1;
			continue;
		}

		// merged_index_lo <= median_indx <= merged_index_hi
		if (merged_size % 2 != 0) return nums2[mid];
		double median = nums2[mid];
		int other = (iters_pair.first == iters_pair.second ? *(iters_pair.first) : *(++iters_pair.first));
		median += min(other,
			(mid + 1 < nums2.size()) ? nums2[mid + 1] : numeric_limits<int>::max());

		return median / 2.0;
	}

	// median is in the first array, either after the place, where to the second arr is merged or before.
	median_index = median_index - (hi == -1 ? 0 : nums2.size());
	return (merged_size % 2 == 0) ? (nums1[median_index] + nums1[median_index + 1]) / 2.0 :
		nums1[median_index];
}

int main() {

	/*vector<int> input{ 0,1,0,2,1,0,1,3,2,1,2,1 };
	cout << trap(input);*/


	vector<vector<char>> input{ 
		{'1','0','1','0','0'},
		{'1','0','1','1','1'},
		{'1','1','1','1','1'},
		{'1','0','0','1','0'} };

	//vector<vector<char>> input2{ {'1'} };
	//cout << maximalRectangle(input);

	//string s = "ADOBECODEBANC", t = "ABC";
	/*string s = "bccbacaaababaabcbabbbbabbcca", t = "caccabbabcacbabcb";
	cout << minWindow(s, t);*/

	//vector<int> n1 = { 1, 1, 2 };
	//vector<int> n2 = { 3, 3, 3, 4 };
	vector<int> n1 = {1,2,4 };
	vector<int> n2 = { 3 };
	cout << findMedianSortedArrays(n1, n2);

	/*BurstingBaloonsSolution sln;
	vector<int> input{ 3,1,5,8 };
	cout << sln.maxCoins(input);*/

	cin.get();
	return 0;
}