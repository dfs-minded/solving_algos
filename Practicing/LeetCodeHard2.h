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
	
	auto merged_size = nums1.size() + nums2.size();
	int median_index = (merged_size - 1) / 2;
	double median = 0;
	int first_arr_median_index = -1;

	int lo = 0, hi = nums1.size() - 1;
	while (lo <= hi) {
		int mid = lo + (hi - lo) / 2;
		auto iters_pair = equal_range(nums2.begin(), nums2.end(), nums1[mid]);
		auto lo_index = distance(nums2.begin(), iters_pair.first);
		auto hi_index = distance(nums2.begin(), iters_pair.second);

		if (lo_index + mid > median_index + 1) { // we do not know we found first or second element of the median
			hi = mid - 1;
			continue;
		}

		if (hi_index + mid < median_index + 1) {
			lo = mid + 1;
			continue;
		}

		// median_index is between found indexes
		if (merged_size % 2 != 0) return nums1[mid];

		if (median == 0) { // search for the second element
			median = nums1[mid];
			first_arr_median_index = mid;
			lo = 0;
			hi = nums1.size() - 1;
			// if found first element of the median
			if (lo_index + mid > median_index && hi_index + mid < median_index)
				++median_index;
			else --median;
		} else {
			median = (median + nums1[mid]) / 2.0;
			return median;
		}
	}
	// did not found either one or both elements in the first array

	if (median == 0) { // both not found
		median_index = (hi == -1) ? median_index : (nums2.size() - 1) / 2;

		return (merged_size % 2 != 0) ? nums2[median_index] 
			: (nums2[median_index] + nums2[median_index + 1]) / 2.0;
	}

	return (median + nums2[median_index - (first_arr_median_index + 1)]) / 2.0;
}


class BurstingBaloonsSolution {
public:
	int maxCoins(const vector<int>& nums) {
		vector<int> nums_no_zeroes(nums.size() + 2);
		int i = 0;
		nums_no_zeroes[i++] = 1;
		for (int j = 0; j < nums.size(); ++j)
			if (nums[j]) nums_no_zeroes[i++] = nums[j];

		nums_no_zeroes[i] = 1;
		max_sum_sub_arr_ = vector(i + 1, vector<double>(i + 1, numeric_limits<double>::min()));
		
		return maxCoins(nums_no_zeroes, 0, i);
	}
private:
	vector<vector<double>> max_sum_sub_arr_;

	int maxCoins(const vector<int>& nums, int lo, int hi) {
		if (lo == hi) return nums[lo];

		if (max_sum_sub_arr_[lo][hi] != numeric_limits<double>::min())
			return max_sum_sub_arr_[lo][hi];

		double max_sum_sofar = numeric_limits<double>::min();
		for (int i = lo + 1; i < hi; ++i) {
			int left_sum = maxCoins(nums, lo, i);
			int right_sum = maxCoins(nums, i, hi);
			double cur_sum = left_sum + right_sum + nums[lo] * nums[i] * nums[hi];
			max_sum_sofar = max(max_sum_sofar, cur_sum);
		}

		max_sum_sub_arr_[lo][hi] = max_sum_sofar;
		return max_sum_sofar;
	}
};

 // Definition for a binary tree node.
  struct TreeNode {
      int val;
      TreeNode *left;
      TreeNode *right;
      TreeNode(int x) : val(x), left(NULL), right(NULL) {}
 };
 
class MinCameraSolution {
	public:
		int minCameraCover(TreeNode* root) {
			if (!root) return 0;
			if (!root->left && !root->right) return 1;

			minCamerasNeeded(root, false, false);
			int w_camera = get<0>(cameras_num_.at(root));
			int wo_camera_observed = get<1>(cameras_num_.at(root));

			return min(w_camera, wo_camera_observed);
		}
	private:
		// tuple of num cameras if we place a camera in a given node, if we don't place and the node is observed  by its
		// parent and if we don't place and the node is not observed by the parent
		unordered_map<TreeNode*, tuple<int, int, int>> cameras_num_;

		int minCamerasNeeded(TreeNode* node, bool observed_by_parent, bool must_place_camera) {
			if (!node) return must_place_camera ? 1 : 0;
			if (!node->left && !node->right)
				return !observed_by_parent || must_place_camera ? 1 : 0;

			if (cameras_num_.count(node) > 0) {
				int w_camera = get<0>(cameras_num_.at(node));
				int wo_camera_observed_by_child = get<1>(cameras_num_.at(node));
				int wo_camera_observed_by_parent = get<2>(cameras_num_.at(node));

				if (must_place_camera) return w_camera;

				if (observed_by_parent) {
					if (w_camera <= min(wo_camera_observed_by_child, wo_camera_observed_by_parent))
						return w_camera;
					return min(wo_camera_observed_by_child, wo_camera_observed_by_parent);
				}
				else return wo_camera_observed_by_child;
			}

			int w_camera = minCamerasNeeded(node->left, true, false) + minCamerasNeeded(node->right, true, false) + 1;

			auto left_subtree_res_w_cam = minCamerasNeeded(node->left, false, true);
			auto left_subtree_res_wo_cam = minCamerasNeeded(node->left, false, false);

			auto right_subtree_res_w_cam = minCamerasNeeded(node->right, false, true);
			auto right_subtree_res_wo_cam = minCamerasNeeded(node->right, false, false);

			int wo_camera_observed_by_child = min({ left_subtree_res_w_cam + right_subtree_res_wo_cam,
												   left_subtree_res_wo_cam + right_subtree_res_w_cam,
												   left_subtree_res_w_cam + right_subtree_res_w_cam });

			int wo_camera_observed_by_parent = min(wo_camera_observed_by_child,
				left_subtree_res_wo_cam + right_subtree_res_wo_cam);

			cameras_num_[node] = { w_camera, wo_camera_observed_by_child, wo_camera_observed_by_parent };

			return minCamerasNeeded(node, observed_by_parent, must_place_camera);
		}
};

using ll = long long;
vector<int> smallestRange(vector<vector<int>>& nums) {
	auto new_end = remove_if(nums.begin(), nums.end(),
		[](const vector<int>& v) { return v.empty(); });
	nums.erase(new_end, nums.end());

	vector<stack<pair<int, int>>> element_to_list_id_and_index(2 * pow(10, 5));

	ll curr_min = numeric_limits<int>::max();
	ll curr_max = numeric_limits<int>::min();

	// add first element of each of K lists
	for (int k = 1; k < nums.size(); ++k) {
		ll curr_val = nums[k][0];
		curr_min = min(curr_min, curr_val);
		curr_max = max(curr_max, curr_val);

		element_to_list_id_and_index[curr_val].push({ k, 1 });
	}

	long long min_dist_sofar = abs(curr_max - curr_min);
	auto best_min = curr_min;
	auto best_max = curr_max;

	while (true) {
		// remove the smallest element and try to find better sln on the right in that list
		auto[list_id, pos] = element_to_list_id_and_index[curr_min].top();
		element_to_list_id_and_index[curr_min].pop();
		if (pos == nums[list_id].size()) break; // reached the end of one of the lists

		ll curr_val = nums[list_id][pos];
		curr_min = min(curr_max, curr_val);
		curr_max = max(curr_max, curr_val);

		if (min_dist_sofar > abs(curr_max - curr_min)) { // found smaller range
			min_dist_sofar = abs(curr_max - curr_min);
			best_min = curr_min;
			best_max = curr_max;
		}

		element_to_list_id_and_index[curr_val].push({ list_id, pos + 1 });
	}

	return { static_cast<int>(best_min), static_cast<int>(best_max) };
}



 // Definition for an interval.
  struct Interval {
      int start;
      int end;
      Interval() : start(0), end(0) {}
      Interval(int s, int e) : start(s), end(e) {}
  };
 
  class InsertIntervalSolution {
  public:
	  vector<Interval> insert(vector<Interval>& intervals, Interval newInterval) {
		  if (intervals.empty()) return { newInterval };

		  Interval merging = newInterval;
		  vector<Interval> res;
		  const Interval kEmptyInterval;

		  for (const auto& interw : intervals) {
			  if (Intersect(merging, interw)) {
				  merging.start = min(merging.start, interw.start);
				  merging.end = max(merging.end, interw.end);
				  continue;
			  }

			  if (!Equal(merging, kEmptyInterval) && merging.end < interw.start) {
				  res.push_back(merging);
				  merging = kEmptyInterval;
			  }

			  res.push_back(interw);
		  }

		  if (Intersect(merging, intervals.back()))
			  res.push_back(merging);

		  return res;
	  }

  private:
	bool Intersect(const Interval& first, const Interval& second) {
		  return (second.start < first.start) ? !(second.end < first.start)
											  : !(first.end < second.start);
	}

	bool Equal(const Interval& lhs, const Interval& rhs) {
		return lhs.start == rhs.start && lhs.end == rhs.end;
	}
};

int main() {

	/*vector<int> input{ 0,1,0,2,1,0,1,3,2,1,2,1 };
	cout << trap(input);*/


	/*vector<vector<char>> input{ 
		{'1','0','1','0','0'},
		{'1','0','1','1','1'},
		{'1','1','1','1','1'},
		{'1','0','0','1','0'} };*/

	//vector<vector<char>> input2{ {'1'} };
	//cout << maximalRectangle(input);

	//string s = "ADOBECODEBANC", t = "ABC";
	/*string s = "bccbacaaababaabcbabbbbabbcca", t = "caccabbabcacbabcb";
	cout << minWindow(s, t);*/

	//vector<int> n1 = { 1, 1, 2 };
	//vector<int> n2 = { 3, 3, 3, 4 };

	//vector<int> n1 = {1,2,4 };
	//vector<int> n2 = { 3 };

	//vector<int> n1 = { };
	//vector<int> n2 = { 3 };

	//vector<int> n1 = { 20 };
	//vector<int> n2 = { 10 };

	//vector<int> n1 = { 3 };
	//vector<int> n2 = { 1,2,3,4 };

	//vector<int> n1 = { 1,2};
	//vector<int> n2 = { 3,4 };

	/*vector<int> n1 = { 1,3 };
	vector<int> n2 = { 2 };*/
	//cout << findMedianSortedArrays(n1, n2);

	/*BurstingBaloonsSolution sln;
	vector<int> input{ 3,1,5,8 };
	cout << sln.maxCoins(input);*/

	/*MinCameraSolution sln;
	auto* n1 = new TreeNode(1);
	auto* n2 = new TreeNode(2);
	auto* n3 = new TreeNode(3);
	auto* n4 = new TreeNode(4);

	n1->left = n2;
	n2->left = n3;
	n3->right = n4;

	cout << sln.minCameraCover(n1);*/

	/*vector<vector<int>> input{
		{4,10,15,24,26},
		{0,9,12,20},
		{5,18,22,30},
		{},
	};

	auto res = smallestRange(input);*/

	InsertIntervalSolution sln;
	vector<Interval> intervals { {1, 2}, {3, 5}, {6, 7},{8, 10},{12, 16} };
	Interval newInterval { 4, 8 };
	auto res = sln.insert(intervals, newInterval);

	cin.get();
	return 0;
}