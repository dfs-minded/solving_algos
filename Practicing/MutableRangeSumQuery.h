class NumArray {
public:
	NumArray(vector<int> nums) : data_(nums) {
		int K = ceil(sqrt(data_.size()));

		int lo = 0;
		int hi = 0;
		int curr_sum = 0;

		while (hi < data_.size()) {
			while (hi < data_.size() && hi - lo < K)
				curr_sum += data_[hi++];
			decomposed_data_.push_back({ lo, hi - 1, curr_sum });
			curr_sum = 0;
			lo = hi;
		}
	}

	void update(int i, int val) {
		for (auto& node : decomposed_data_) {
			if (node.lo <= i && node.hi >= i) {
				int diff = data_[i] - val;
				node.sum -= diff;
				data_[i] = val;
				break;
			}
		}
	}


	int sumRange(int i, int j) {
		int included_lo = data_.size(), included_hi = -1;
		int total_sum = 0;

		for (auto& node : decomposed_data_) {
			if (node.lo >= i && node.hi <= j) {
				total_sum += node.sum;
				included_lo = min(included_lo, node.lo);
				included_hi = max(included_hi, node.hi);
			}
		}

		while (i < min(j + 1, included_lo)) total_sum += data_[i++];
		if (included_hi != -1)
			while (j > included_hi) total_sum += data_[j--];
		return total_sum;
	}

	void PrintBlocks() {
		for (auto& block : decomposed_data_)
			cout << block.lo << " " << block.hi << " " << block.sum << endl;
	}
private:
	struct SqrtDecNode {
		int lo;
		int hi;
		int sum;
		SqrtDecNode(int l, int r, int s) : lo(l), hi(r), sum(s) {}
	};

	vector<int> data_;
	vector<SqrtDecNode> decomposed_data_;
};


int main() {
	//["NumArray", "sumRange", "sumRange", "sumRange", "update", "update", "update", "sumRange", "update", "sumRange", "update"]
	//[[[0, 9, 5, 7, 3]], [4, 4], [2, 4], [3, 3], [4, 5], [1, 7], [0, 8], [1, 2], [1, 9], [4, 4], [3, 4]]
	//[null, 3, 15, 7, null, null, null, 12, null, 5, null]
	
	vector<int> input{ 0, 9, 5, 7, 3 };
	NumArray arr(input);
	cout << arr.sumRange(4, 4) << endl;
	cout << arr.sumRange(2, 4) << endl;
	cout << arr.sumRange(3, 3) << endl;
	arr.update(4, 5);
	arr.update(1, 7);
	arr.update(0, 8);
	cout << arr.sumRange(1, 2) << endl;

	cin.get();
	return 0;
}