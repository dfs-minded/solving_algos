#pragma once

class QueueToShaman {
public:
	enum class QueryType {
		NormalPriorityPush = 0, HighPriorityPush = 1, GoToShamanPop = 2
	};

	void ComputeQueueOrder(QueryType type, int& next_visitor) {
		if (type == QueryType::GoToShamanPop) {
			auto& next = top_queue_.front();
			top_queue_.pop_front();
			next_visitor = next;
		}
		else {
			if (type == QueryType::NormalPriorityPush)
				bottom_queue_.push_back(last_num_in_queue_++);
			else if (type == QueryType::HighPriorityPush)
				bottom_queue_.push_front(last_num_in_queue_++);
			
		}

		// rebalance queues
		if (bottom_queue_.size() > top_queue_.size() - 1) {
			top_queue_.push_back(bottom_queue_.front());
			bottom_queue_.pop_front();
		}
	}

private:
	deque<int> top_queue_;
	deque<int> bottom_queue_;
	int last_num_in_queue_{ 0 };
	
};

int main() {
	int query_type;
	int next_visitor = -1;
	QueueToShaman queue_to_shaman;
	while (cin >> query_type) {
		auto type = static_cast<QueueToShaman::QueryType>(query_type);
		queue_to_shaman.ComputeQueueOrder(type, next_visitor);
		if (type == QueueToShaman::QueryType::GoToShamanPop)
			cout << next_visitor << endl;
	}

	return 0;
}