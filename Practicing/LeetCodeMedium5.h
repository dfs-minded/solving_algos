#pragma once

int totalFruit(vector<int>& tree) {
	int best_sum_sofar{ 0 };
	int lo{ 0 }; // points to the first fruit we have added into the basket
	int hi{ 0 }; // points to the fruit we want to add
	set<int> fruit_types;

	vector<int> last_seen(tree.size() + 1, -1);

	while (hi < tree.size()) {

		// move |hi| poiter
		while (hi < tree.size() && (fruit_types.size() <= 1 || fruit_types.count(tree[hi]))) {
			fruit_types.insert(tree[hi]);
			last_seen[tree[hi]] = hi;
			++hi;
		}

		// update |best_sum_sofar|
		best_sum_sofar = max(best_sum_sofar, hi - lo);

		// move |lo| pointer
		int type_to_remove = *fruit_types.begin();
		int min_index = last_seen[type_to_remove];

		for (auto type : fruit_types) {
			if (last_seen[type] < min_index) {
				min_index = last_seen[type];
				type_to_remove = type;
			}			
		}

		fruit_types.erase(type_to_remove);
		lo = min_index + 1;
	}

	return best_sum_sofar;
}

vector<int> splitIntoFibonacci(const string& s) {
	vector<int> res;

	for (int i = 0; i < 10 && i < s.size() - 2; ++i) {
		long long f_0 = 0;
		for (int d = 0; d <= i; ++d)
			f_0 = f_0 * 10 + (s[d] - '0');

		for (int j = i + 1; j - i <= 10 && j < s.size() - 1; ++j) {
			long long f_1 = s[i + 1] - '0';
			for (int d = i + 2; d <= j; ++d)
				f_1 = f_1 * 10 + (s[d] - '0');

			res.push_back(static_cast<int>(f_0));
			res.push_back(static_cast<int>(f_1));

			for (int k = j + 1; k < s.size();) {
				long long expected_f_2 = res[res.size() - 1] + 0ll + res[res.size() - 2];
				long long f_2 = s[k++] - '0';
				while (k < s.size() && f_2 < expected_f_2)
					f_2 = f_2 * 10 + (s[k++] - '0');

				if (f_2 > numeric_limits<int>::max() || f_2 != expected_f_2) {
					res = {};
					break;
				}

				res.push_back(static_cast<int>(f_2));
			}
			if (res.size() > 2) return res;
			if (f_1 == 0) break;
		}
		if (f_0 == 0) break;
	}
	return res;
}

string reorganizeString(const string& s) {
	constexpr int kBase = 26;
	array<int, kBase> letters_count;
	fill_n(letters_count.begin(), kBase, 0);
	for (auto c : s) ++letters_count[c - 'a'];

	priority_queue<pair<int, char>> heap;
	for (int i = 0; i < kBase; ++i)
		if (letters_count[i]) heap.push({ letters_count[i], 'a' + i });

	string ans = "";
	while (!heap.empty()) {
		auto[most_frequent_count, most_frequent_char] = heap.top();
		heap.pop();
		ans += most_frequent_char;

		if (heap.empty())
			return (most_frequent_count > 1) ? "" : ans;

		auto[second_most_frequent_count, second_most_frequent_char] = heap.top();
		heap.pop();
		ans += second_most_frequent_char;

		if (most_frequent_count - 1)
			heap.push({ most_frequent_count - 1, most_frequent_char });
		if (second_most_frequent_count - 1)
			heap.push({ second_most_frequent_count - 1, second_most_frequent_char });
	}
	return ans;
}

class AddBoldTagSolution {
public:
	string addBoldTag(string s, vector<string>& dict) {
		constexpr char const *kBoldOpenTag = "<b>";
		constexpr char const *kBoldCloseTag = "</b>";

		vector<int> pattern_positions(s.size() + 1);

		for (auto& word : dict)
		  FindPatternStartEndPos(s, word, &pattern_positions);

		string wrapped_res = "";

		int curr_tags_balance = 0;
		for (int i = 0; i < s.size(); ++i) {
			if (pattern_positions[i] > 0) {
				if (curr_tags_balance == 0) wrapped_res += string(kBoldOpenTag);
				curr_tags_balance += pattern_positions[i];
			}
			else  if (pattern_positions[i] < 0) {
				curr_tags_balance += pattern_positions[i];
				if (curr_tags_balance == 0) wrapped_res += string(kBoldCloseTag);
			}

			wrapped_res += s[i];
		}
		if (pattern_positions.back() < 0) wrapped_res += string(kBoldCloseTag);
		return wrapped_res;
	}
private:
	void FindPatternStartEndPos(const string& t, const string& p, vector<int>* pattern_positions) {
		auto pi_function = CalculatePiFinction(t, p);

		for (int i = 0; i < pi_function.size(); ++i) {
			if (pi_function[i] == p.size()) {
				pattern_positions->at(i - p.size() + 1) += 1;
				pattern_positions->at(i + 1) += -1;
			}
		}
	}

	vector<int> CalculatePiFinction(const string& t, const string& p) {
		const string kDelim{ "#" };
		auto text = p + kDelim + t;
		vector<int> pi_function(text.size());

		for (int i = 1; i < text.size(); ++i) {
			int j = pi_function[i - 1];

			while (j && text[j] != text[i])
				j = pi_function[j - 1];

			if (text[i] == text[j]) pi_function[i] = j + 1;
			// else - 0
		}

		return vector<int>(pi_function.begin() + p.size() + kDelim.size(), pi_function.end());
	}
};

int longestPalindromeSubseq(const string& s) {
	string t(s);
	reverse(t.begin(), t.end());
	const int kN = s.size();
	vector<vector<int>> longest_palindr_dp(kN + 1, vector<int>(kN + 1));

	for (int i = 1; i <= kN; ++i) {
		for (int j = 1; j <= kN; ++j) {
			int max_sofar = 0;
			if (s[i - 1] == t[j - 1]) max_sofar = max(max_sofar, longest_palindr_dp[i - 1][j - 1] + 1);
			else
				max_sofar = max({ max_sofar, longest_palindr_dp[i][j - 1], longest_palindr_dp[i - 1][j] });

			longest_palindr_dp[i][j] = max_sofar;
		}
	}

	return longest_palindr_dp.back().back();
}

int wordsTyping(const vector<string>& sentence, int rows, int cols) {
	const int N = sentence.size();
	vector<int> next_word(N);
	vector<int> sentences_fit(N);

	for (int word_index = 0; word_index < N; ++word_index) {
		int num_sentences_fit = 0;
		int curr_col = 0;
		int curr_word_index = word_index;

		while (curr_col + sentence[curr_word_index].size() <= cols) {
			curr_col += sentence[curr_word_index].size() + 1;
			++curr_word_index;

			if (curr_word_index == N) {
				++num_sentences_fit;
				curr_word_index = 0;
			}
		}

		next_word[word_index] = curr_word_index;
		sentences_fit[word_index] = num_sentences_fit;
	}

	int total_sentences_fit = 0;
	int curr_word_index = 0;

	for (int row = 0; row < rows; ++row) {
		total_sentences_fit += sentences_fit[curr_word_index];
		curr_word_index = next_word[curr_word_index];
	}
	return total_sentences_fit;
}

int strStr(string haystack, string needle) {
	vector<int> pi_function(needle.size());
	for (int i = 1; i < needle.size(); ++i) {
		int j = pi_function[i - 1];
		while (j && needle[i] != needle[j])
			j = pi_function[j - 1];
		pi_function[i] = (needle[i] == needle[j]) ? j + 1 : 0;
	}

	int prev_pi_func = 0;
	int i = 0;
	for (; i < haystack.size() && prev_pi_func < needle.size(); ++i) {
		int curr_pi_func = prev_pi_func;
		while (curr_pi_func && haystack[i] != needle[curr_pi_func])
			curr_pi_func = pi_function[curr_pi_func - 1];

		prev_pi_func = (haystack[i] == needle[curr_pi_func]) ? curr_pi_func + 1 : 0;
	}

	return (prev_pi_func == needle.size()) ? i - prev_pi_func + 1 : -1;
}

int main() {
	/*vector<int> input{ 0,1,1,4,3 };
	cout << totalFruit(input);*/

	//auto res = splitIntoFibonacci("0000");

	//auto res = reorganizeString("aab");

	//AddBoldTagSolution sln;

	//vector<string> dict{ "aaa","aab","bc","aaabbcc" };
	//cout << sln.addBoldTag("aaabbcc", dict) << endl; // => "<b>aaabbcc</b>"

	//vector<string> dict2{ "aaa", "aab", "bc" };
	//cout << sln.addBoldTag("aaabbcc", dict2) << endl; // => "<b>aaabbc</b>c"
	

	//cout << longestPalindromeSubseq("bbab");


	/*vector<string> input{ "f", "p", "a" };
	cout << wordsTyping(input, 8, 7);*/

	cout << strStr("hello", "ll");

	cin.get();
	return 0;
}
