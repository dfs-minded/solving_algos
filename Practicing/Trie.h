#pragma once

#include <unordered_map>

using namespace std;

template <typename TInput, typename TSymbol>
class Trie {
public:
	Trie(TSymbol root_val) : root_(new Node(root_val)) {}

	void Add(TInput key) {
		root_ = Add(key, key[0], 0, root_);
	}

	void Delete(TInput key) {
		root_ = Delete(key, 0, root_);
	}

	TInput BiggestXORNum(TInput key) {
		int pos = 0;
		auto curr_node = root_;
		TInput last_key_on_path;
		TInput curr_prefix;

		TSymbol next_symbol = key[pos];
		while (curr_node) {
			if (curr_node->KeysNum)
				last_key_on_path = curr_prefix;

			if (pos == key.size()) break;

			TSymbol negated = next_symbol == '0' ? '1' : '0';

			if (curr_node->Next.count(negated) != 0)
				next_symbol = negated; // want to choose opposite for bigger XOR

			curr_prefix += next_symbol;

			curr_node = curr_node->Next.empty() ? nullptr : curr_node->Next[next_symbol];
			++pos;
			if (pos < key.size()) next_symbol = key[pos];
		}

		return last_key_on_path;
	}

private:
	struct Node {
		int KeysNum = 0;
		TSymbol Val;
		unordered_map<TSymbol, Node*> Next;

		Node(TSymbol val) : Val(val) {};
	};

	Node* root_ = nullptr;

	Node* Add(TInput key, TSymbol val, int pos, Node* node) {
		if (node == nullptr) node = new Node(val);

		if (pos == key.size()) {
			++node->KeysNum;
			return node;
		}

		auto c = key[pos]; // use pos key symbol to identify subtree
		node->Next[c] = Add(key, c, pos + 1, node->Next[c]);
		return node;
	}

	Node* Delete(TInput key, int curr_pos, Node* node) {
		if (!node) return node;

		if (curr_pos < key.size()) {
			auto next = Delete(key, curr_pos + 1, node->Next[key[curr_pos]]);
			if (!next) node->Next.erase(key[curr_pos]);

			if (node->Next.size() == 0 && node != root_) { // no children and not a root
				delete node;
				node = nullptr;
			}

			return node;
		}

		// last symbol in the key:
		--node->KeysNum;

		if (!node->KeysNum && node->Next.size() == 0) { // no children
			delete node;
			node = nullptr;
		}

		return node;
	}
};