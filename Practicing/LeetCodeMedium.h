#pragma once

void generate(int open_count, int close_count, string curr, vector<string>* res) {
	if (!open_count && !close_count) {
		res->push_back(curr);
		return;
	}

	if (open_count)
		generate(open_count - 1, close_count, curr + '(', res);
	
	if (close_count > open_count)
		generate(open_count, close_count - 1, curr + ')', res);
}

vector<string> generateParenthesis(int n) {
	vector<string> res;
	string curr;

	generate(n, n, curr, &res);
	return res;
}

int main()
{
	auto res = generateParenthesis(0);
	int u; cin >> u;
	return 0;
}