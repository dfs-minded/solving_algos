#pragma once

template <typename TInput, typename TSymbol>
class Trie {
public:
	Trie(TSymbol root_val) : root_(new Node(false, root_val)) {}

	void Add(TInput key) {
		root_ = Add(key, 0, root_);
	}
	 
	void Delete(TInput key) {
		root_ = Delete(key, 0, root_);
	}

	TInput BiggestXOR(TInput key) {
		int pos = 0;
		auto curr_node = root_;
		TInput last_key_on_path;
		TInput curr_prefix;

		while (curr_node && !curr_node->Next.empty() && pos < key.size()) {
			TSymbol next_symbol = key[pos];
			TSymbol negated = (static_cast<int>(key[pos]) + 1) % 2;
			
			if (curr_node->Next.count(negated) != 0)
				next_symbol = negated; // want to choose opposite for bigger XOR
			
			curr_prefix += next_symbol;
			if (curr_node->HasKey)
				last_key_on_path = curr_prefix;
			curr_node = curr_node->Next[next_symbol];
			++pos;
		}

		return last_key_on_path;
	}

private:
	struct Node {
		bool HasKey;
		TSymbol Val;
		unordered_map<TSymbol, Node*> Next;

		Node(bool has_key, TSymbol val) : HasKey(has_key), Val(val) {};
	};
	
	Node* root_ = nullptr;

	Node* Add(TInput key, int curr_pos, Node* node) {
		if (node == nullptr)
			node = new Node(curr_pos == key.size() - 1, key[curr_pos]);
		
		if (curr_pos == key.size() - 1) node->HasKey = true;
		else node->Next[key[curr_pos]] = Add(key, curr_pos + 1, node->Next[key[curr_pos]]);
		
		return node;
	}

	Node* Delete(TInput key, int curr_pos, Node* node) {
		if (curr_pos < key.size() - 1) {
			auto next = Delete(key, curr_pos + 1, node->Next[key[curr_pos]]);
			if (!next) node->Next.erase(key[curr_pos]);

			if (node->Next.size() == 0) { // no children
				delete node;
				node = nullptr;
			}

			return node;
		}
		
		// last symbol in the key:
		node->HasKey = false;

		if (node->Next.size() == 0) { // no children
			delete node;
			node = nullptr;
		}

		return node;
	}
};