#pragma once

struct ListNode {
    int val;
    ListNode *next;
    ListNode(int x) : val(x), next(nullptr) {}
 };

int Length(ListNode* A) {
	int res = 0;

	while (A) {
		++res;
		A = A->next;
	}

	return res;
}

ListNode* Reverse(ListNode* A) {
	ListNode* prev = nullptr;
	ListNode* curr = A;

	while (curr) {
		auto tmp = curr->next;
		curr->next = prev;
		prev = curr;
		curr = tmp;
	}

	return prev;
}

int isPalin(ListNode* A) {
	int len = Length(A);
	auto* B = A;
	for (int i = 0; i < (len + 1) / 2; ++i)
		B = B->next;

	auto R = Reverse(B);

	while (R) {
		if (A->val != R->val) return false;
		A = A->next;
		R = R->next;
	}

	return true;
}


int main() {
	ListNode n1(1);
	ListNode n2(2);
	ListNode n3(1);
	n1.next = &n2;
	n2.next = &n3;

	cout << isPalin(&n1);

	int o; cin >> o;
	return 0;
}