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

int main() {
	vector<int> input{ 2,1,5,6,2,3 };
	cout << MaxAreaHistograms(input);

	int o; cin >> o;
}