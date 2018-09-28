#pragma once

// LC, e
int HammingDist(int x, int y) {
	int mask = 1;
	int res = 0;

	for (int i = 0; i < 32; ++i, mask << 1)
		res += (x & mask) != (y & mask);

	return res;
}

// LC, e
vector<int> twoSum(vector<int>& nums, int target) {
	unordered_map<int, int> nums_hash;

	for (int i = 0; i < nums.size(); ++i) {
		int lookup = target - nums[i];
		if (nums_hash.count(lookup))
			return { nums_hash[lookup], i };

		nums_hash[nums[i]] = i;
	}

	return { -1, -1 };
}

// LC, e
struct ListNode {
	int val;
	ListNode *next;
	ListNode(int x) : val(x), next(NULL) {}
};
// iterative
ListNode* reverseList(ListNode* head) {
	if (!head) return head;
	ListNode* prev = nullptr;
	auto* curr = head;

	while (curr) {
		auto* tmp = curr->next;
		curr->next = prev;
		prev = curr;
		curr = tmp;
	}

	return curr;
}
// recursive
ListNode* reverseListRecursive(ListNode* prev, ListNode* curr) {
	auto* new_head = curr;
	if (curr->next)
		new_head = reverseListRecursive(curr, curr->next);
	
	curr->next = prev;
	return new_head;
}
ListNode* reverseListRec(ListNode* head) {
	if (!head || !head->next) return head;

	auto* new_head = reverseListRecursive(head, head->next);
	head->next = nullptr;
	return new_head;
}


int singleNumber(vector<int>& nums) {
	int res = 0;

	for (auto n : nums) res ^= n;

	return res;
}


int findMaxArea(vector<vector<int>>& grid, int r, int c) {
	int area = 0;
	queue<pair<int, int>> to_process;
	to_process.push({ r, c });

	while (!to_process.empty()) {
		tie(r, c) = to_process.front();
		to_process.pop();
		if (grid[r][c] != 1) continue;

		grid[r][c] = -1;
		++area;
		
		if (r > 0 && grid[r - 1][c] == 1)
			to_process.push({ r - 1, c });
		if (c > 0 && grid[r][c - 1] == 1)
			to_process.push({ r, c - 1 });
		if (r < grid.size() - 1 && grid[r + 1][c] == 1)
			to_process.push({ r + 1, c });
		if (c < grid[0].size() - 1 && grid[r][c + 1] == 1)
			to_process.push({ r, c + 1 });
	}

	return area;
}
int maxAreaOfIsland(vector<vector<int>>& grid) {
	int max_sofar = 0;

	for (int r = 0; r < grid.size(); ++r) {
		for (int c = 0; c < grid[0].size(); ++c) {
			if (grid[r][c] == 1) {
				auto area = findMaxArea(grid, r, c);
				max_sofar = max(max_sofar, area);
			}
		}
	}

	return max_sofar;
}


vector<int> findAnagrams(string s, string p) {
	vector<int> res;
	if (s.size() < p.size()) return res;

	vector<int> sc(26);
	vector<int> pc(26);

	for (auto c : p) ++pc[c - 'a'];

	for (int i = 0; i < p.size(); ++i) ++sc[s[i] - 'a'];

	for (int j = p.size(); j < s.size(); ++j) {
		if (sc == pc) res.push_back(j - p.size());
		--sc[s[j - p.size()] - 'a'];
		++sc[s[j] - 'a'];
	}

	if (sc == pc) res.push_back(s.size() - p.size());
	return res;
}



int calcLen(ListNode* node) {
	int len = 0;
	while (node) {
		++len;
		node = node->next;
	}

	return len;
}

ListNode* advance(ListNode* node, int steps) {
	while (steps--) node = node->next;
	return node;
}

ListNode* reverse(ListNode* node) {
	auto* prev = node;
	auto* curr = node->next;

	while (curr) {
		auto* next = curr->next;
		curr->next = prev;
		prev = curr;
		curr = next;
	}

	return prev;
}

bool isPalindrome(ListNode* node1, ListNode* node2, int num) {
	while (num--) {
		if (node1->val != node2->val) return false;
		node1 = node1->next;
		node2 = node2->next;
	}

	return true;
}

// LC, e
bool isPalindrome(ListNode* head) {
	if (!head) return true;

	int len = calcLen(head);
	if (len == 1) return true;

	auto* mid = advance(head, (len + 1) / 2);
	auto* reversed_half = reverse(mid);
	return isPalindrome(head, reversed_half, len / 2);
}


// Fibonacci Matrix Solution
using Matrix = vector<vector<int>>;

Matrix multiply(Matrix A, Matrix B) {
	Matrix C(2, vector<int>(2));

	C[0][0] = A[0][0] * B[0][0] + A[0][1] * B[1][0];
	C[0][1] = A[0][0] * B[0][1] + A[0][1] * B[1][1];
	C[1][0] = A[1][0] * B[0][0] + A[1][1] * B[1][0];
	C[1][1] = A[1][0] * B[0][1] + A[1][1] * B[1][1];

	return C;
}

Matrix fastPow(Matrix A, int p) {
	if (p == 0) return { { 1, 0 },{ 0, 1 } };

	if (p % 2 == 0) {
		auto M = fastPow(A, p / 2);
		return multiply(M, M);
	}

	return multiply(A, fastPow(A, p - 1));
}

int fibo(int n) {
	if (n == 0) return 0;

	Matrix Q_0 = { { 1, 1 },{ 1, 0 } };
	auto Q_n = fastPow(Q_0, n);
	return Q_n[0][0];
}


int main()
{
	/*vector<int> nums = { 3,2,4 };
	auto res = twoSum(nums, 6);*/

	/*vector<vector<int>> input = { {1, 1, 0, 0, 0}, {1, 1, 0, 0, 0}, {0, 0, 0, 1, 1}, {0, 0, 0, 1, 1} };
	vector<vector<int>> input_large = { {0, 0, 0, 0, 1, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 1, 1}, { 0,1,0,0,0,0,1,1,1,0,1,1,0,0,0,1,0,1,1,1,0,0,1,0,1,0,0,1,0,0,0,1,1,1,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,1 }, { 1,0,0,0,0,0,0,1,1,1,1,0,1,1,0,0,0,0,1,0,0,1,0,0,1,1,1,0,0,0,0,0,1,1,1,0,0,1,1,1,0,1,1,0,1,0,1,1,1,1 }, { 0,0,0,0,1,1,1,0,1,1,0,1,0,0,0,0,0,0,0,1,0,1,0,1,1,0,0,0,0,0,1,1,0,0,1,1,1,0,1,1,1,1,0,1,1,0,0,0,1,1 }, { 0,0,0,0,1,1,1,1,0,1,1,0,0,1,1,1,0,1,1,1,0,1,1,0,0,0,1,0,1,1,1,1,1,0,1,1,0,1,1,0,1,0,0,0,0,0,1,0,0,1 }, { 0,0,1,1,1,0,1,0,0,0,1,0,0,0,0,0,1,0,1,0,0,1,0,1,0,0,1,1,1,0,1,1,1,1,1,0,1,0,1,1,1,1,0,0,0,0,0,1,0,1 }, { 0,1,0,1,0,0,1,0,0,0,0,1,1,1,1,1,0,1,1,1,1,1,0,0,0,0,1,0,0,1,0,0,1,0,0,1,0,0,1,1,0,0,0,0,1,1,0,1,1,0 }, { 0,1,0,0,1,1,1,1,0,1,1,0,1,0,1,0,0,0,1,0,0,0,0,0,1,0,1,1,0,1,0,0,1,0,1,1,1,0,1,0,0,1,1,1,0,0,1,0,1,0 }, { 0,1,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,1,0,1,0,1,0,0,0,1,0,0,0,1,0,1,0,0,1,0,0,0,0,0,0,1,1,0,1,0,0,1,0,0 }};
	auto res = maxAreaOfIsland(input_large);*/

	/*string s = "abab";
	string p = "ab";
	auto r = findAnagrams(s, p);*/

	//ListNode ln1(1);
	//ListNode ln2(2);
	//ListNode ln3(2);
	//ListNode ln4(1);
	//ln1.next = &ln2;
	//ln2.next = &ln3;
	//ln3.next = &ln4;
	//auto res = isPalindrome(&ln1);


	cout << fibo(3);

	int n; cin >> n;
	return 0;
}

