
 // Definition for singly-linked list.
  struct ListNode {
      int val;
     ListNode *next;
     ListNode(int x) : val(x), next(NULL) {}
 };
 
class Solution {
public:
	ListNode* reverseKGroup(ListNode* head, int k) {
		ListNode* first = head;
		ListNode* second = head;
		for (int i = 1; i < k && second; ++i) second = second->next;

		if (!second) return first;
		ListNode* new_head = nullptr;
		ListNode* new_tail = nullptr;

		while (true) {
			auto[curr_head, rest] = Reverse(first, second);
			if (!new_head) new_head = curr_head;

			if (new_tail) new_tail->next = curr_head;
			new_tail = first;

			first = rest;
			second = first;
			for (int i = 1; i < k && second; ++i) second = second->next;
			if (!second) {
				new_tail->next = first;
				break;
			}
		}

		return new_head;
	}
private:
	// returns new head and the rest of the list
	pair<ListNode*, ListNode*> Reverse(ListNode* first, ListNode* last) {
		ListNode* prev = nullptr;
		ListNode* curr = first;
		auto* last_node = last->next;

		while (curr && curr != last_node) {
			auto* tmp = curr->next;
			curr->next = prev;
			prev = curr;
			curr = tmp;
		}

		return { prev, curr };
	}
};

int main() {
	ListNode n1(1);
	ListNode n2(2);
	ListNode n3(3);
	ListNode n4(4);
	ListNode n5(5);

	n1.next = &n2;
	n2.next = &n3;
	n3.next = &n4;
	n4.next = &n5;

	Solution sln;
	auto* res = sln.reverseKGroup(&n1, 2);

	cin.get();
	return 0;
}