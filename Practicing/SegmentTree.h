#pragma once
class SegmentTree {
public:
	const int kTreeMemoryMultiplier{ 4 };

	SegmentTree(const vector<int>& data) : data_(data), segment_tree_(kTreeMemoryMultiplier * data.size()) {
		Build(1, 0, data_.size() - 1);
	}

	void Update(int index, int new_val) {
		int delta = new_val - data_[index];
		data_[index] = new_val;
		Update(index, delta, 1, 0, data_.size() - 1);
	}

	int GetRangeSum(int lo, int hi) {
		return GetRangeSum(lo, hi, 1, 0, data_.size() - 1);
	}

private:
	vector<int> data_;
	vector<int> segment_tree_;

	void Build(int v, int lo, int hi) {
		if (lo == hi) {
			segment_tree_[v] = data_[lo];
			return;
		}
		int mid = (lo + hi) / 2;
		Build(2 * v, lo, mid);
		Build(2 * v + 1, mid + 1, hi);

		segment_tree_[v] = segment_tree_[2 * v] + segment_tree_[2 * v + 1];
	}

	void Update(int index, int delta, int v, int v_lo, int v_hi) {
		segment_tree_[v] += delta;

		if (v_lo == v_hi) return;

		int mid = (v_lo + v_hi) / 2;
		if (index <= mid) Update(index, delta, 2 * v, v_lo, mid);
		else Update(index, delta, 2 * v + 1, mid + 1, v_hi);
	}

	int GetRangeSum(int target_lo, int target_hi, int v, int v_lo, int v_hi) {
		if (v_lo >= target_lo && v_hi <= target_hi) return segment_tree_[v];

		int mid = (v_lo + v_hi) / 2;
		if (target_lo <= mid && target_hi > mid)
			return GetRangeSum(target_lo, mid, 2 * v, v_lo, mid) + // left child 
			GetRangeSum(mid + 1, target_hi, 2 * v + 1, mid + 1, v_hi); // right child
		if (target_hi <= mid) 
		    return GetRangeSum(target_lo, target_hi, 2 * v, v_lo, mid);
		return GetRangeSum(target_lo, target_hi, 2 * v + 1, mid + 1, v_hi);
	}
};

class NumArray {
public:
	NumArray(vector<int>& nums) : segment_tree_(nums) {}

	void update(int i, int val) {
		segment_tree_.Update(i, val);
	}

	int sumRange(int i, int j) {
		return segment_tree_.GetRangeSum(i, j);
	}

private:
	SegmentTree segment_tree_;
};

int main() {
	//["NumArray", "sumRange", "update", "sumRange"]
	//[[[1, 3, 5]], [0, 2], [1, 2], [0, 2]]
	/*vector<int> input{ 1, 3, 5 };
	NumArray nr(input);
	cout << nr.sumRange(0, 2) << endl;
	nr.update(1, 2);
	cout << nr.sumRange(0, 2) << endl;*/

	//["NumArray", "sumRange", "sumRange", "sumRange", "update", "update", "update", "sumRange", "update", "sumRange", "update"]
	//[[[0, 9, 5, 7, 3]], [4, 4], [2, 4], [3, 3], [4, 5], [1, 7], [0, 8], [1, 2], [1, 9], [4, 4], [3, 4]]
	// expected: [null,3,15,7,null,null,null,12,null,5,null]
	//vector<int> input{ 0, 9, 5, 7, 3 };
	//NumArray nr(input);
	//cout << nr.sumRange(4, 4) << endl;
	//cout << nr.sumRange(2, 4) << endl;
	//cout << nr.sumRange(3, 3) << endl;
	//nr.update(4, 5);
	//nr.update(1, 7);
	//nr.update(0, 8);
	//cout << nr.sumRange(1, 2) << endl;


	//["NumArray", "update", "update", "update", "sumRange", "update", "sumRange", "update", "sumRange", "sumRange", "update"]
	//[[[7, 2, 7, 2, 0]], [4, 6], [0, 2], [0, 9], [4, 4], [3, 8], [0, 4], [4, 1], [0, 3], [0, 4], [0, 4]]
	vector<int> input{ 7, 2, 7, 2, 0 };
	NumArray nr(input);
	nr.update(4, 6);
	nr.update(0, 2);
	nr.update(0, 9);
	cout << nr.sumRange(4, 4) << endl;
	nr.update(3, 8);
	cout << nr.sumRange(0, 4) << endl;

	cin.get();
}