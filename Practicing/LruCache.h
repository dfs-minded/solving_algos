class LRUCache1 {
public:
	LRUCache1(int capacity) : capacity_(capacity) {}

	int get(int key) {
		if (data_.count(key) == 0) return -1;
		key_to_timestamp_[key] = ++timestamp_;
		timestamp_to_key_pq_.push({ timestamp_, key });
		return data_[key];
	}

	void put(int key, int value) {
		if (data_.count(key) == 0 && data_.size() == capacity_) {
			if (timestamp_to_key_pq_.empty()) return;
			auto to_delete = timestamp_to_key_pq_.top();
			auto key_to_delete = std::get<1>(to_delete);
			timestamp_to_key_pq_.pop();

			while (data_.count(key_to_delete) == 0 || key_to_timestamp_[key_to_delete] != std::get<0>(to_delete)) {
				to_delete = timestamp_to_key_pq_.top();
				key_to_delete = std::get<1>(to_delete);
				timestamp_to_key_pq_.pop();
			}

			data_.erase(key_to_delete);
			key_to_timestamp_.erase(key_to_delete);
		}

		data_[key] = value;
		key_to_timestamp_[key] = ++timestamp_;
		timestamp_to_key_pq_.push({ timestamp_, key });
	}

private:
	int capacity_;
	long long timestamp_{ numeric_limits<long long>::min() };
	unordered_map<int, int> data_;
	unordered_map<int, long long> key_to_timestamp_;

	using QueryData = tuple<long long, int>;
	priority_queue<QueryData, vector<QueryData>, std::greater<QueryData>> timestamp_to_key_pq_;
};

class LRUCache {
public:
	LRUCache(int capacity) : kCapacity(capacity) {}

	int get(int key) {
		if (key_to_node_map_.count(key) == 0) return -1;
		SetNewHead(key);
		return key_to_node_map_[key]->val;
	}

	void put(int key, int value) {
		if (key_to_node_map_.count(key) != 0) {
			SetNewHead(key);
			UpdateVal(key, value);
			return;
		}

		if (key_to_node_map_.size() == kCapacity) DeleteLRU();

		auto* new_node = new Node(key, value);
		key_to_node_map_[key] = new_node;
		SetNewHead(key);
	}

private:
	struct Node {
		Node *next{ nullptr };
		Node *prev{ nullptr };
		int key{ -1 };
		int val{ -1 };

		constexpr Node(int k, int v) : key(k), val(v) {}
	};

	const int kCapacity;

	// nullptr<-head<->node<->node<->tail->nullptr
	Node *head_{ nullptr }, *tail_{ nullptr };
	unordered_map<int, Node*> key_to_node_map_;

	void SetNewHead(int key) {
		auto* node = key_to_node_map_[key];
		if (node == head_) return;

		auto* left = node->prev;
		auto* right = node->next;
		if (left) left->next = right;
		if (right) right->prev = left;

		if (node == tail_) tail_ = left;
		if (!tail_) tail_ = node;
		node->next = head_;
		if (head_) head_->prev = node;
		node->prev = nullptr;
		head_ = node;
	}

	void UpdateVal(int key, int value) {
		key_to_node_map_[key]->val = value;
	}

	void DeleteLRU() {
		if (!tail_) return;
		key_to_node_map_.erase(tail_->key);
		auto* prev = tail_->prev;

		if (prev) prev->next = nullptr;
		else head_ = nullptr;

		delete tail_;
		tail_ = prev;
	}
};

int main() {
	LRUCache c(2);

	/*["LRUCache", "put", "put", "get", "put", "get", "put", "get", "get", "get"]
	[[2],		   [1, 1], [2, 2], [1], [3, 3], [2], [4, 4], [1], [3], [4]]*/
	/*c.put(1, 1);
	c.put(2, 2);
	cout << c.get(1) << endl;
	c.put(3, 3);
	cout << c.get(2) << endl;
	c.put(4, 4);
	cout << c.get(1) << endl;
	cout << c.get(3) << endl;
	cout << c.get(4) << endl;*/
	// expected: [1,-1,-1,3,4]



	//["LRUCache", "put", "put", "get", "put", "put", "get"]
	//[[2], ``````[2, 1], [2, 2], [2], [1, 1], [4, 1], [2]]

	//c.put(2, 1);
	//c.put(2, 2);
	//cout << c.get(2) << endl;
	//c.put(1, 1);
	//c.put(4, 1);
	//cout << c.get(2) << endl;

	// expected: [2,-1]


	/*["LRUCache", "get", "put", "get", "put", "put", "get", "get"]
		[[2], [2], [2, 6], [1], [1, 5], [1, 2], [1], [2]]*/
	cout << c.get(2) << endl;
	c.put(2, 6);
	cout << c.get(1) << endl;
	c.put(1, 5); 
	c.put(1, 2);
	cout << c.get(1) << endl;
	cout << c.get(2) << endl;

	// expected: [-1,-1,2,6]

	/*["LRUCache", "put", "get", "put", "get", "get"]
	[[1], [2, 1], [2], [3, 2], [2], [3]]*/
	/*LRUCache c1(1);
	c1.put(2, 1);
	cout << c1.get(2) << endl;;
	c1.put(3, 2);
	cout << c1.get(2) << endl;;
	cout << c1.get(3) << endl;;*/

	cin.get();
	return 0;
}