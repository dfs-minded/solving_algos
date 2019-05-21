#pragma once
class Solution {
public:
	int cherryPickup(vector<vector<int>>& grid) {
		const int R = grid.size();
		const int C = grid[0].size();

		// dp[r1][c1][r2]; c2 = r1 + c1 - r2;
		auto max_can_collect_dp = vector<vector<vector<int>>>(R, vector<vector<int>>(C, vector<int>(R)));
		max_can_collect_dp[0][0][0] = grid[0][0];

		for (int r1 = 0; r1 < R; ++r1) {
			for (int c1 = 0; c1 < C; ++c1) {
				for (int r2 = 0; r2 < R; ++r2) {
					if (!r1 && !c1 && !r2) continue;
					int c2 = r1 + c1 - r2;
					if (c2 || c2 >= C) continue;
					if (grid[r1][c1] == -1 || grid[r2][c2] == -1) {
						max_can_collect_dp[r1][c1][r2] = -1;
						continue;
					}

					int max_from_prev_step = (!r1 || !c1 || !r2 || !c2) ? 0 : -1;
					if (r1 - 1 >= 0) // first came from top, second - from left 
						max_from_prev_step = max(max_from_prev_step, max_can_collect_dp[r1 - 1][c1][r2]);

					if (c1 - 1 >= 0) // first came from left, second - from left 
						max_from_prev_step = max(max_from_prev_step, max_can_collect_dp[r1][c1 - 1][r2]);

					if (r2 - 1 >= 0 && r1 - 1 >= 0)  // first came from top, second - from top 
						max_from_prev_step = max(max_from_prev_step, max_can_collect_dp[r1 - 1][c1][r2 - 1]);

					if (r2 - 1 >= 0 && c1 - 1 >= 0)  // first came from left, second - from top 
						max_from_prev_step = max(max_from_prev_step, max_can_collect_dp[r1][c1 - 1][r2 - 1]);

					if (max_from_prev_step == -1) {
						max_can_collect_dp[r1][c1][r2] = -1;
						continue;
					}

					max_can_collect_dp[r1][c1][r2] = max_from_prev_step + grid[r1][c1];
					if (c1 != c2) max_can_collect_dp[r1][c1][r2] += grid[r1][c1];
				}
			}
		}

		return max_can_collect_dp.back().back().back();
	}
};

int oddEvenJumps(const vector<int>& A) {
	if (A.empty()) return 0;
	const int N = A.size();
	// store indices
	vector<int> greater_or_eq(N, -1);
	vector<int> smaller_or_eq(N, -1);

	set<pair<int, int>> val_to_index;

	for (int i = N - 1; i >= 0; --i) {
		auto first_eq_or_greater_iter = val_to_index.lower_bound({ A[i], 0 });

		if (first_eq_or_greater_iter != val_to_index.end())
			greater_or_eq[i] = first_eq_or_greater_iter->second;

		if (first_eq_or_greater_iter != val_to_index.end() && first_eq_or_greater_iter->first == A[i])
			smaller_or_eq[i] = first_eq_or_greater_iter->second;
		else if (first_eq_or_greater_iter != val_to_index.begin()) {
			--first_eq_or_greater_iter; // first smaller now
			auto nearest_smaller_iter = val_to_index.lower_bound({ first_eq_or_greater_iter->first, 0});
			smaller_or_eq[i] = nearest_smaller_iter->second;
		}

		val_to_index.insert({ A[i], i });
	}

	vector<bool> odd_jump_dp(N, -1);
	vector<bool> even_jump_dp(N, -1);
	odd_jump_dp.back() = true;
	even_jump_dp.back() = true;
	int good_starting_indices_num = 1; // one for the last number in array

	for (int i = N - 2; i >= 0; --i) {
		odd_jump_dp[i] = greater_or_eq[i] == -1 ? false : even_jump_dp[greater_or_eq[i]];
		if (i > 0)
			even_jump_dp[i] = smaller_or_eq[i] == -1 ? false : odd_jump_dp[smaller_or_eq[i]];

		if (odd_jump_dp[i]) ++good_starting_indices_num;
	}

	return good_starting_indices_num;
}


class ShortestPalindromeSolution1 {
public:
	string shortestPalindrome(string s) {
		if (s.empty()) return s;
		auto text = s;
		reverse(text.begin(), text.end());
		auto pi_function = GetPiFunction(text, s);
		auto max_chars_palindromic_from_end = pi_function.back();

		auto palindromic_res = s;
		for (int i = max_chars_palindromic_from_end; i < s.size(); ++i)
			palindromic_res = s[i] + palindromic_res;

		return palindromic_res;
	}

private:
	static constexpr char kPiFuncDelim{ '#' };

	vector<int> GetPiFunction(const string& text, const string& pattern) {
		auto search_text = pattern + kPiFuncDelim + text;
		vector<int> pi_function(search_text.size());

		for (int i = 1; i < search_text.size(); ++i) {
			int j = pi_function[i - 1];

			while (j && search_text[j] != search_text[i])
				j = pi_function[j - 1];

			if (search_text[j] == search_text[i])
				pi_function[i] = j + 1;
		}

		return vector(pi_function.end() - text.size(), pi_function.end());
	}
};

class ShortestPalindromeSolution {
public:
	string shortestPalindrome(const string& s) {
		auto max_chars_palindromic_from_end = CalcLongestPalindromicPrefixLenght(s);
		return to_string(max_chars_palindromic_from_end);
		auto palindrome_part_to_append = s.substr(max_chars_palindromic_from_end);
		reverse(palindrome_part_to_append.begin(), palindrome_part_to_append.end());
		return palindrome_part_to_append + s;
	}

private:
	int CalcLongestPalindromicPrefixLenght(const string& text) {
		vector<int> pi_function(text.size() + 1);

		// calc pi-function for the pattern
		for (int i = 1; i < text.size(); ++i) {
			int j = pi_function[i - 1];

			while (j != 0 && text[j] != text[i])
				j = pi_function[j - 1];

			pi_function[i] = (text[j] == text[i]) ? j + 1 : 0;
		}

		// search text is reversed pattern, so just go from right to left:
		int prev_pi_func_val = 0;
		for (int i = text.size() - 1; i >= 0; --i) {
			int curr_pi_func_val = prev_pi_func_val;

			while (curr_pi_func_val != 0 && text[curr_pi_func_val] != text[i])
				curr_pi_func_val = pi_function[curr_pi_func_val - 1];

			prev_pi_func_val = (text[curr_pi_func_val] == text[i]) ? curr_pi_func_val + 1 : 0;
		}

		return prev_pi_func_val;
	}
};

class CountPalindromicSubsequencesSolution {
public:
	static constexpr long long kModulo = static_cast<long long>(1e9 + 7);

	int countPalindromicSubsequences(string S) {
		const int N = S.size();
		vector<vector<long long>> palindroms_count_dp(N + 1, vector<long long>(N + 1));

		auto closest_on_lhs = GetSameClosestCharOnTheLeft(S);
		auto closest_on_rhs = GetSameClosestCharOnTheRight(S);

		for (int i = N; i > 0; --i) {
			palindroms_count_dp[i][i] = 1;
			for (int j = i + 1; j <= N; ++j) {
				if (S[i - 1] != S[j - 1]) {
					
					palindroms_count_dp[i][j] = palindroms_count_dp[i][j - 1] 
						- palindroms_count_dp[i + 1][j - 1] + palindroms_count_dp[i + 1][j];
					palindroms_count_dp[i][j] %= kModulo;
					if (palindroms_count_dp[i][j] > kModulo)
						cout << palindroms_count_dp[i][j];
					if (palindroms_count_dp[i][j] < 0) {
						cout << palindroms_count_dp[i][j - 1] << endl;
						cout << palindroms_count_dp[i + 1][j] << endl;
						cout << palindroms_count_dp[i + 1][j - 1] << endl;
						cout << palindroms_count_dp[i + 1][j - 1] % kModulo << endl;
					}
					continue;
				}

				int closes_left_ind = closest_on_lhs[j - 1];
				int closes_right_ind = closest_on_rhs[i - 1];

				if (closes_left_ind == i - 1) { // no same letter in between
					palindroms_count_dp[i][j] = 2 * palindroms_count_dp[i + 1][j - 1] + 2/* for 'a' and 'aa' palindromes */;
				}
				else if (closes_left_ind == closes_right_ind) { // one same letter in between
					palindroms_count_dp[i][j] = 2 * palindroms_count_dp[i + 1][j - 1] + 1/* for 'aa' palindrome */;
				}
				else { // two same letters in between
					palindroms_count_dp[i][j] = 2 * palindroms_count_dp[i + 1][j - 1] -
						palindroms_count_dp[closes_left_ind + 1][closes_right_ind - 1];
				}

				palindroms_count_dp[i][j] %= kModulo;
				if (palindroms_count_dp[i][j] > kModulo)
					cout << palindroms_count_dp[i][j];
			}
		}

		return palindroms_count_dp[1].back();
	}

private:
	vector<int> GetSameClosestCharOnTheLeft(const string& s) {
		vector<int> closest_on_lhs(s.size(), -1);

		for (int i = 1; i < s.size(); ++i) {
			for (int j = i - 1; j >= 0; --j) {
				if (s[i] == s[j]) {
					closest_on_lhs[i] = j;
					break;
				}
			}
		}
		return closest_on_lhs;
	}

	vector<int> GetSameClosestCharOnTheRight(const string& s) {
		vector<int> closest_on_rhs(s.size(), -1);

		for (int i = 0; i < s.size(); ++i) {
			for (int j = i + 1; j < s.size(); ++j) {
				if (s[i] == s[j]) {
					closest_on_rhs[i] = j;
					break;
				}
			}
		}
		return closest_on_rhs;
	}
};


int main() {
	/*Solution sln;
	vector<vector<int>> grid{ {0,1,-1},{1,0,-1},{1,1,1} };
	cout << sln.cherryPickup(grid);*/

	/*vector<int> input{2,3,1,1,4};
	cout << oddEvenJumps(input);*/

	//ShortestPalindromeSolution sln;
	//cout << sln.shortestPalindrome("abacd");

	CountPalindromicSubsequencesSolution sln;
	cout << sln.countPalindromicSubsequences("bdccbdbdcaacaadbdacaadabbdaababacbdbadaccccbbbccbdbddcacdbbcdaaababddadbbcabaddbbaabcdacdcddcbacdcadddbaadcbbccdcbdcaaabcbddacaccadacdcadacadccabdbadcdabaaccbcabbacdcbbbabddbcccbcbcddacaccabbabccdbbabacadbbdbcccbddaaadcdaddcccddaacbddabddabadbcbaadcaddbdccabaddcccdccbacaccccbcdccacbabddcbcbbbdbbddaabbcaddbddddaacbbccbcdbadacccbadbccdddadcbacbbdcabcbdbbaabcdcbdacabacdccdaabcbbddbdddcacdbdccbbabbaccdcbabcdacdabdddaddacdbddabcbbbdcccacbdadddbbdbcabccadbcbadddcbcddaababcabbbcbbabadaddabcbdacaadcababcbacadcdabcdbddadaadddddbaaacddaacadcdbaaacbacaabdabccbacabbabddacddcdacdcddbacbddbdcadcddabdadcdbcbbcadbcdabbbbdcdadadbbcadbcbbcccccabdaabcdacbdbcadabcdbaaaaacdaaabcdcaabddcacdcacabbaadbdbcbaaabadbdadadaccddacababbdcbaaaccaabbdaddadcabaabbdaadadcabcabbdbbdccbabbbabbcbcbdcacccbdbabdbaadccdaddaddaadadccabdcbbbccacbbaaabccaccdcabbbddbdbbacdaabbdcdacbdddbacbadccabbbdbbbdbdbcadcacddddcdbdcabccdccabaadbcbacaaababdddcbcdaaabaaabddbcabcbbcdddccdcbccbbccadccaaccccbdadcaacdcbcabacacadaabacddaadcccbcabdad");

	cin.get();
	return 0;
}