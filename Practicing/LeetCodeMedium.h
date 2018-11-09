#pragma once

using uint64 = unsigned long long;
const int kModulo = 1e9 + 7;

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

uint64 sumOfSubarrayMins(const vector<int>& input) {
	uint64 res = 0;

	// precalculate "wings":
	// number of values on the left till first smaller
	vector<int> greater_left(input.size());
	// number of values on the right, till first smaller
	vector<int> greater_or_eq_right(input.size());

	stack<int> increasing;
	for (int i = 0; i < input.size(); ++i) {
		while (!increasing.empty() && input[increasing.top()] >= input[i])
			increasing.pop();
		if (!increasing.empty()) greater_left[i] = i - increasing.top() - 1;
		else greater_left[i] = i;
		increasing.push(i);
	}

	increasing = {};
	for (int j = input.size() - 1; j >= 0; --j) {
		while (!increasing.empty() && input[increasing.top()] > input[j])
			increasing.pop();
		if (!increasing.empty()) greater_or_eq_right[j] = increasing.top() - j - 1;
		else greater_or_eq_right[j] = input.size() - j - 1;
		increasing.push(j);
	}

	// consider the number of sub-arrays, where curr element will be minimal
	for (int i = 0; i < input.size(); ++i) {
		// multiply the number of such sub-array begins on the number of ends
		res += (((greater_left[i] + 1) * (greater_or_eq_right[i] + 1) % kModulo) * input[i]) % kModulo;
	}

	return res;
}

int longestValidParentheses(const string& s) {
	// pair of balance to index
	stack<pair<int,int>> balance;
	balance.push({ 0, 0 });
	int max_len = 0;

	int curr_balance = 0;
	for (auto i = 0; i < s.size(); ++i) {
		if (s[i] == '(') ++curr_balance;
		else --curr_balance;

		// remove all greater elements from stack
		while (balance.size() > 1 && balance.top().first > curr_balance)
			 balance.pop();

		if (curr_balance < 0) {
			balance.top().second = i + 1; // only {0, i} left on stack
			curr_balance = 0;
		}
		else if (curr_balance == balance.top().first) // we get 0 if subtract, means we got the correct sub-array of parentheses
			max_len = max(max_len, i - balance.top().second + 1);
		else balance.push({ curr_balance , i + 1});
	}

	return max_len;
}

int numberOfArithmeticSlices(vector<int>& A) {
	if (A.size() < 3) return 0;
	int res = 0;
	int curr_diff = A[1] - A[0];
	int curr_len = 2; // number of elements in a sequence

	for (int i = 2; i < A.size(); ++i) {
		if (A[i] - A[i - 1] == curr_diff) ++curr_len;
		else { // other diff
			if (curr_len == 3) res += 1;
			else if (curr_len > 3) {
				int a_n = curr_len - 2;
				res += (1 + a_n) * a_n / 2;
			}

			curr_diff = A[i] - A[i - 1];
			curr_len = 2;
		}
	}

	if (curr_len == 3) res += 1;
	else if (curr_len > 3) {
		int a_n = curr_len - 2;
		res += (1 + a_n) * a_n / 2;
	}

	return res;
}

vector<int> dailyTemperatures(vector<int>& T) {
	vector<int> res(T.size());
	stack<pair<int, int>> greater_temp; // pair of temperature to # smaller, which were popped from stack

	for (int i = T.size() - 1; i >= 0; --i) {
		int days_to_wait = 0;

		while (!greater_temp.empty() && greater_temp.top().first <= T[i]) {
			days_to_wait += greater_temp.top().second + 1;
			greater_temp.pop();
		}

		// if there were no days with greater temperature, assign 0;
		if (greater_temp.empty())
			greater_temp.push({ T[i], 0 });
		else {
			greater_temp.push({ T[i], days_to_wait });
			res[i] = days_to_wait + 1;  // plus one day from today
		}
	}

	return res;
}

int findLongestChain(vector<vector<int>>& pairs) {
	sort(pairs.begin(), pairs.end());

	// last elements for each length
	vector<int> last_elem = { max(pairs[0][0], pairs[0][1]) };

	for (int i = 1; i < pairs.size(); ++i) {
		int new_last_elem = max(pairs[i][0], pairs[i][1]);
		
		int diff = pairs[i][0] - last_elem.back();
		if (diff >= 1) last_elem.push_back(new_last_elem); // extend, append new length
		else last_elem.back() = min(last_elem.back(), new_last_elem); // update
	}

	return last_elem.size();
}

template<class Iter>
void fill(Iter&& begin, Iter&& end, char c) {
	while (begin < end) {
		*begin = c;
		++begin;
	}
}

string pushDominoes(string dominoes) {
	int left = 0;
	// process left pointer till we find first letter
	while (left < dominoes.size() && dominoes[left] == '.') ++left;
	if (left == dominoes.size()) return dominoes;

	// now left points to the letter, 
	if (dominoes.front() == '.' && dominoes[left] == 'L') // " . . . L"
		fill(dominoes.begin(), dominoes.begin() + left, 'L');

	int right = 0;

	while (true) {
		// process right pointer till we find next letter
		right = left + 1;
		while (right < dominoes.size() && dominoes[right] == '.') ++right;
		if (right == dominoes.size()) break;

		// now right points to the letter and we can fill dots between left and right
		if (dominoes[left] == dominoes[right])
			fill(dominoes.begin() + left + 1, dominoes.begin() + right, dominoes[left]);
		else if (dominoes[left] == 'R' && dominoes[right] == 'L') {
			int half_end = (right - left + 1) / 2;
			fill(dominoes.begin() + left + 1, dominoes.begin() + left + half_end, 'R');

			int half_begin = (right - left) / 2 + 1;
			fill(dominoes.begin() + left + half_begin, dominoes.begin() + right, 'L');
		}

		// process left pointer till we find next letter
		++left;
		while (left < dominoes.size() && dominoes[left] == '.') ++left;
		if (left == dominoes.size()) break;
	}

	if (dominoes[left] == 'R' && dominoes.back() == '.') // "R . . ."
		fill(dominoes.begin() + left + 1, dominoes.end(), 'R');

	return dominoes;
}

int minPathSum(const vector<vector<int>>& grid) {
	vector<int> prev_row(grid[0].size(), numeric_limits<int>::max());
	vector<int> curr_row;

	// traverse row by row;
	for (int r = 0; r < grid.size(); ++r) {
		curr_row = grid[r];

		for (int c = 0; c < grid[0].size(); ++c) {
			if (!r && !c) continue;
			int left_min = c > 0 ? curr_row[c - 1] : numeric_limits<int>::max();
			curr_row[c] += min(prev_row[c], left_min);
		}

		prev_row = curr_row;
	}

	return curr_row.back();
}

// Stone Game LC 887

// Scores of the first and the second players correspondingly.
struct PlayersScores {
	int First = 0;
	int Second = 0;

	bool operator != (PlayersScores&& other) {
		return First != other.First || Second != other.Second;
	}
};
vector<vector<PlayersScores>> memo; // left index in rows, right index in columns

PlayersScores solveStoneGame(int l, int r, const vector<int>& piles) {
	if (l == r) return { 0, piles[l] };
	if (memo[l][r] != PlayersScores()) return memo[l][r];

	auto scores1 = solveStoneGame(l + 1, r, piles); // take left pile
	auto scores2 = solveStoneGame(l, r - 1, piles); // take right pile

	if ((r - l + 1) % 2 == 0) { // first player move
		int s1 = piles[l] + scores1.First;
		int s2 = piles[r] + scores2.First;
		if (s1 > s2) memo[l][r] = { s1, scores1.Second };
		else memo[l][r] = { s2, scores2.Second };
	} else { // second player move
		int s1 = piles[l] + scores1.Second;
		int s2 = piles[r] + scores2.Second;
		if (s1 > s2) memo[l][r] = { scores1.First, s1 };
		else memo[l][r] = { scores2.First, s2 };
	}

	return memo[l][r];
}
bool stoneGame(vector<int>& piles) {
	memo.resize(piles.size(), vector<PlayersScores>(piles.size(), PlayersScores()));
	auto scores = solveStoneGame(0, piles.size() - 1, piles);
	return scores.First > scores.Second;
}

int lengthOfLIS(vector<int>& nums) {
	vector<int> last_elem;

	for (int i = 0; i < nums.size(); ++i) {
		if (last_elem.empty() || last_elem.back() < nums[i]) last_elem.push_back(nums[i]);
		else {
			auto first_greater_or_eq_iter = lower_bound(last_elem.begin(), last_elem.end(), nums[i]);
			*first_greater_or_eq_iter = nums[i];
		}
	}

	return last_elem.size();
}


bool isSubsequence(string s, string t) {
	vector<vector<int>> indices(26);
	for (int i = 0; i < t.size(); ++i)
		indices[t[i] - 'a'].push_back(i);

	int last_letter_indx = -1;
	for (auto c : s) {
		auto letters_indices = indices[c - 'a'];
		auto first_greater_iter = upper_bound(letters_indices.begin(), letters_indices.end(), last_letter_indx);
		if (first_greater_iter == letters_indices.end()) return false;
		last_letter_indx = *first_greater_iter;
	}

	return true;
}


// z*n vector
vector<vector<int>> dp;

int solveFindMaxForm(const vector<string>& strs, int i, int z, int n) {
	if (i < 0) return 0;
	if (dp[z][n] != -1) return dp[z][n];

	int zeroes = 0;
	int ones = 0;
	for (auto c : strs[i]) {
		if (c == '0') ++zeroes;
		else ++ones;
	}

	int res1 = 0;
	if (zeroes <= z && ones <= n) // can form
		res1 = solveFindMaxForm(strs, i - 1, z - zeroes, n - ones) + 1; // form
	int res2 = solveFindMaxForm(strs, i - 1, z, n); // do not form

	dp[z][n] = max(res1, res2);
	return dp[z][n];
}

int findMaxForm(vector<string>& strs, int z, int n) {
	dp = vector<vector<int>>(z + 1, vector<int>(n + 1, -1));
	dp[0][0] = 0;
	
	return solveFindMaxForm(strs, strs.size() - 1, z, n);
}

int CalcMaxProduct(const vector<int>& nums, int l, int r, int negatives_num) {
	if (l == r - 1) return nums[l];
	if (negatives_num % 2 == 0)
		return accumulate(nums.begin() + l, nums.begin() + r, 1, multiplies<int>());

	int left_product = 1;
	while (nums[l] > 0) left_product *= nums[l++];
	if (negatives_num > 1) left_product *= nums[l++];

	int right_product = 1;
	while (nums[--r] > 0)
		right_product *= nums[r--];
	if (negatives_num > 1) right_product *= nums[r];

	int res = accumulate(nums.begin() + l, nums.begin() + r, 1, multiplies<int>());

	res *= (right_product > left_product ? right_product : left_product);

	return res;
}

int maxProduct(vector<int>& nums) {
	nums.push_back(0); // safeguard
	int negatives_num = 0;
	int res = 0;
	int l = 0;
	for (int r = 0; r < nums.size(); ++r) {
		if (nums[r] < 0) ++negatives_num;
		else if (nums[r] == 0) {
			res = max(res, CalcMaxProduct(nums, l, r, negatives_num));
			l = r + 1;
		}
	}

	return res;
}


int main()
{
	//auto res = generateParenthesis(0);
	//vector<int> input{ 29959,29867,29822,29704,29676,29650,29577,29488,29286,29255,29232,29207,29071,29034,28925,28849,28731,28693,28624,28606,28591,28397,28357,28308,28295,28210,28119,28090,28004,27903,27845,27830,27777,27736,27640,27540,27506,27428,27341,27308,27182,27152,27122,27029,26928,26872,26796,26765,26663,26597,26580,26530,26498,26475,26436,26406,26382,26312,26213,26134,26088,26025,25943,25912,25875,25845,25810,25702,25638,25614,25531,25524,25488,25470,25444,25402,25283,25262,25121,24988,24958,24886,24769,24697,24635,24595,24490,24456,24453,24346,24313,24248,24200,24148,24107,24052,24044,24021,23970,23908,23897,23835,23752,23741,23714,23661,23596,23545,23509,23470,23439,23409,23350,23215,23166,23155,23100,23024,22923,22825,22793,22627,22613,22536,22450,22383,22312,22268,22205,22175,22136,22028,21971,21900,21824,21769,21726,21583,21546,21513,21494,21428,21327,21264,21254,21174,21140,21112,21000,20921,20902,20830,20817,20783,20735,20657,20616,20573,20485,20378,20363,20305,20259,20210,20114,20002,19846,19785,19747,19667,19645,19622,19610,19580,19542,19516,19454,19392,19310,19277,19194,19131,19090,19004,18883,18845,18791,18781,18668,18591,18518,18475,18368,18331,18310,18287,18217,18114,18092,18048,17990,17964,17912,17836,17740,17704,17630,17613,17573,17428,17356,17341,17300,17260,17180,17174,17126,17071,17041,16866,16850,16828,16672,16618,16577,16499,16407,16357,16318,16293,16202,16150,16075,16041,15948,15921,15844,15843,15785,15764,15668,15626,15579,15473,15387,15255,15190,15139,15062,14996,14954,14918,14907,14902,14867,14851,14817,14799,14751,14720,14536,14506,14474,14353,14303,14280,14185,14107,14012,13932,13858,13781,13585,13563,13533,13451,13412,13362,13249,13208,13181,13064,13037,12961,12926,12892,12786,12731,12611,12573,12506,12502,12496,12470,12443,12370,12262,12182,12153,12069,12000,11847,11806,11781,11708,11687,11593,11550,11445,11372,11329,11308,11291,11268,11241,11191,11027,10982,10879,10862,10776,10695,10603,10502,10464,10350,10338,10305,10273,10176,10124,10094,10038,9953,9935,9812,9786,9743,9728,9508,9472,9383,9349,9236,9215,9130,9124,9042,9008,8988,8901,8833,8809,8780,8716,8580,8462,8334,8321,8305,8280,8257,8246,8137,8077,8043,8016,7984,7955,7927,7906,7746,7663,7653,7572,7542,7530,7489,7420,7390,7361,7337,7245,7210,7188,7175,7096,6898,6846,6745,6675,6569,6478,6427,6363,6284,6260,6243,6206,6154,6135,6078,6061,6017,5995,5917,5863,5836,5793,5763,5743,5678,5572,5532,5459,5384,5341,5299,5251,5231,4995,4933,4861,4740,4672,4625,4496,4445,4361,4282,4215,4135,4097,4028,3917,3862,3711,3553,3498,3410,3388,3384,3288,3279,3244,3221,3181,3171,3150,3060,3035,2975,2965,2834,2760,2637,2584,2533,2440,2383,2311,2285,2255,2211,2192,2121,2054,2010,1964,1850,1724,1642,1577,1411,1409,1332,1296,1265,1256,1220,1195,937,903,880,811,739,720,650,609,547,533,459,434,384,279,231,163,102,78,30,5,52,100,155,217,277,328,389,446,473,546,583,649,702,734,768,857,882,912,1043,1219,1243,1258,1290,1325,1359,1409,1567,1642,1679,1726,1873,1965,2017,2088,2172,2204,2226,2273,2288,2316,2434,2522,2558,2622,2678,2790,2933,2965,3025,3037,3071,3167,3180,3194,3233,3269,3282,3383,3387,3401,3465,3528,3595,3801,3910,4020,4078,4128,4213,4271,4295,4420,4472,4612,4663,4739,4845,4891,4980,5109,5241,5284,5335,5379,5388,5478,5546,5639,5705,5751,5766,5803,5855,5879,5975,6000,6024,6070,6093,6137,6156,6212,6256,6276,6304,6421,6441,6537,6614,6743,6844,6893,7087,7169,7183,7200,7237,7262,7352,7376,7398,7441,7491,7541,7564,7602,7656,7707,7814,7924,7940,7958,8014,8036,8048,8132,8141,8250,8279,8288,8321,8331,8374,8515,8655,8723,8807,8825,8878,8953,8990,9011,9077,9128,9172,9219,9276,9383,9420,9499,9535,9736,9744,9801,9900,9951,10038,10093,10119,10147,10265,10301,10314,10340,10456,10499,10564,10622,10767,10802,10876,10882,10997,11063,11217,11243,11276,11299,11314,11365,11407,11456,11587,11627,11705,11751,11792,11831,11901,12012,12118,12180,12240,12296,12385,12469,12473,12497,12503,12537,12578,12723,12778,12858,12901,12936,13020,13048,13136,13195,13232,13325,13377,13424,13493,13547,13564,13724,13856,13911,13938,14075,14151,14234,14300,14353,14395,14499,14507,14705,14724,14796,14802,14823,14858,14882,14905,14914,14936,14962,15049,15114,15161,15237,15272,15399,15565,15587,15666,15749,15778,15830,15843,15864,15928,16039,16075,16141,16163,16246,16315,16333,16389,16415,16526,16601,16650,16798,16845,16861,16991,17046,17090,17140,17178,17186,17292,17305,17343,17419,17456,17610,17617,17693,17728,17783,17909,17918,17970,18032,18083,18104,18114,18223,18296,18330,18363,18428,18496,18578,18660,18733,18782,18792,18861,18929,19069,19127,19184,19269,19279,19355,19394,19494,19539,19559,19599,19612,19643,19666,19745,19760,19815,19864,20012,20141,20231,20270,20330,20370,20380,20500,20595,20617,20690,20751,20811,20824,20843,20910,20925,21044,21126,21165,21198,21260,21280,21343,21467,21505,21531,21564,21640,21755,21817,21885,21929,22010,22103,22159,22196,22229,22270,22368,22414,22515,22570,22615,22630,22806,22864,22951,23030,23107,23155,23191,23226,23399,23438,23464,23487,23524,23559,23634,23667,23719,23747,23764,23869,23901,23936,24012,24022,24045,24074,24141,24185,24204,24272,24327,24452,24455,24490,24560,24615,24641,24734,24815,24890,24963,25025,25242,25282,25283,25414,25446,25475,25489,25527,25586,25636,25640,25771,25844,25848,25883,25923,26005,26048,26106,26157,26312,26359,26395,26429,26465,26491,26513,26558,26584,26601,26667,26770,26864,26900,26996,27118,27129,27176,27272,27313,27389,27478,27517,27580,27700,27761,27811,27844,27848,27967,28051,28108,28176,28264,28302,28332,28380,28525,28591,28617,28681,28727,28744,28874,28994,29047,29123,29221,29239,29274,29347,29493,29596,29668,29694,29717,29847,29871 };
	/*vector<int> input1{ 3, 1, 2, 4 };
	vector<int> input2{ 48, 87, 27 };
	cout << sumOfSubarrayMins(input1);*/

	//string s1 = "(()";
	//string s2 = ")()())";
	//string s3 = "()";
	//cout << longestValidParentheses(s3);

	/*vector<int> input{ 1,2,3,4 };
	cout << numberOfArithmeticSlices(input);*/

	/*vector<int> input{ 73,74,75,71,69,72,76,73 };
	auto res = dailyTemperatures(input);*/

	//vector<vector<int>> input = { { 4,1 },{ 4,2 },{ 4,6 },{ 7,8 } };
	//vector<vector<int>> input = { { 3,4 },{ 2,3 },{ 1,2 } };
	//vector<vector<int>> input = { {-10, -8}, { 8,9 }, { -5,0 }, { 6,10 }, { -6,-4 }, { 1,7 }, { 9,10 }, { -4,7 } };
	//cout << findLongestChain(input);

	/*string dominoes = ".L.R...LR..L..";
	string expected = "LL.RR.LLRRLL..";
	cout << (pushDominoes(dominoes) == expected) << endl;
	string dominoes2 = "RR.L";
	string expected2 = "RR.L";
	cout << (pushDominoes(dominoes2) == expected2) << endl;
	string dominoes3 = "R.R.L";
	string expected3 = "RRR.L";
	cout << (pushDominoes(dominoes3) == expected3) << endl;
	string dominoes4 = ".R..L.";
	string expected4 = ".RRLL.";
	cout << (pushDominoes(dominoes4) == expected4) << endl;*/

	/*vector<vector<int>> input = { { 1,2 },{ 1,1 } };
	cout << minPathSum(input);*/

	/*vector<int> input = { 5,3, 4, 5};
	cout << stoneGame(input);*/

	//vector<int> input = { 4,10,4,3,8,9 };
	//cout << lengthOfLIS(input);


	/*string s = "acb";
	string t = "ahbgdc";
	cout << isSubsequence(s, t);*/

	//vector<string> input = { "10","0001","111001","1","0" };
	//cout << findMaxForm(input, 5, 3);

	vector<int> input = { 2,3,-2,4 };
	cout << maxProduct(input);

	int u; cin >> u;
	return 0;
}