#pragma once

int SolveNumberOfOnesInAllNums(vector<int> dp, int N) {
	if (N == 0 || N == 1) return N;

	int k = 0; // index of the most significant bit in N
	while (N >> k) ++k;
	--k;

	// Compute number of ones for all numbers, which have smaller the most significant bit
	for (int i = 1; i < k; ++i) { // for each i-th set bit
		dp[i] = dp[i - 1] // number of ones in smaller task
			+ pow(2, i) // number of ones in i-th bit
			+ dp[i - 1]; // number of ones on the right from the i-th bit. Basically, there is exactly
						 // the same bits pattern as for the task of size i-1.
	}

	// Compute number of ones for the smaller number (by removing the most significant bit)
	N &= ~(1 << k);

	return dp[k - 1] 
		+ (N + 1) // number of ones in smaller numbers with the same most significant bit  
		+ SolveNumberOfOnesInAllNums(dp, N);
}

int NumberOfOnesInAllNums(int N) {
	vector<int> dp(N + 1, -1); // number of ones where the most significant i-th bit is equal to 1
	dp[0] = 1;
	
	return SolveNumberOfOnesInAllNums(dp, N);
}


int NearestPowOf2(long long num, bool down) {
	double res = log2(num);
	int integer_res = res;
	if (down) return integer_res;

	if (res - integer_res < numeric_limits<double>::epsilon())
		return integer_res;
	return integer_res + 1;
}

int GetMostSignificantBit(int num) {
	int res = -1;
	while (num) {
		++res;
		num /= 2;
	}
	return res;
}

long long SetMostSignificantBitToZero(int num) {
	int msb = GetMostSignificantBit(num);
	return num & (~(1 << msb));
}

long long GetNumOfSetBitsForPow2Number(int p) {
	// width is equal to p, height is equal to number. Set bits are half from this rectangle,
	// so divide by 2 and get pow(2, p - 1). Plus one is for the most significant bit, where all others are 0. 
	return p * pow(2, p - 1) + 1;
}

long long NumberOfSetBitsBetween(int L, int R) {
	if (!L && !R) return 0;

	// TODO: need to check if there is at all at least one power of 2 between numbers and
	// handle it correctly.
	// TODO handle the case, when L == 0

	// numbers follow in this order: [L, pow(2, nearest_pow_of_2_up), pow(2, nearest_pow_of_2_down), R]
	int nearest_pow_of_2_up = NearestPowOf2(L, false);
	int nearest_pow_of_2_down = NearestPowOf2(R, true);

	long long ans = 0;
	// count the number of set bits between powers of 2 of L and R:
	long long res1 = GetNumOfSetBitsForPow2Number(nearest_pow_of_2_up) - 1;
	long long res2 = GetNumOfSetBitsForPow2Number(nearest_pow_of_2_down);
	ans += res2 - res1;

	// count the number of bits between [L, pow(2, nearest_pow_of_2_up)] 

	// count the number of the most significant bit:
	long long number_on_the_right_from_msb = SetMostSignificantBitToZero(L);
	long long nearest_pow2_number = pow(2, nearest_pow_of_2_up);
	long long msb_count = nearest_pow2_number - 1 - number_on_the_right_from_msb;
	ans += msb_count;

	// recursively solve the problem for the number on the right from the most significant bit in L:
	long long x = SetMostSignificantBitToZero(nearest_pow2_number - 1);
	ans += NumberOfSetBitsBetween(number_on_the_right_from_msb, x);

	// count the number of bits between [pow(2,nearest_pow_of_2_down), R]
	number_on_the_right_from_msb = SetMostSignificantBitToZero(R);
	ans += number_on_the_right_from_msb + 1; // number of the most significant bit
	number_on_the_right_from_msb = SetMostSignificantBitToZero(R);
	ans += NumberOfSetBitsBetween(0, R);

	return ans;
}

int main() {
	//cout << NumberOfOnesInAllNums(0) << endl;
	//cout << NumberOfOnesInAllNums(1) << endl;
	//cout << NumberOfOnesInAllNums(2) << endl;
	//cout << NumberOfOnesInAllNums(3) << endl;
	//cout << NumberOfOnesInAllNums(4) << endl;
	//cout << NumberOfOnesInAllNums(5) << endl;
	//cout << NumberOfOnesInAllNums(6) << endl;
	//cout << NumberOfOnesInAllNums(7) << endl;
	//cout << NumberOfOnesInAllNums(8) << endl;

	//cout << NumberOfSetBitsBetween(0, 0);
	//cout << NumberOfSetBitsBetween(0, 1);
	//cout << NumberOfSetBitsBetween(0, 2);
	//cout << NumberOfSetBitsBetween(0, 3);
	//cout << NumberOfSetBitsBetween(0, 4);
	//cout << NumberOfSetBitsBetween(0, 5);
	//cout << NumberOfSetBitsBetween(0, 6);
	//cout << NumberOfSetBitsBetween(0, 7);
	//cout << NumberOfSetBitsBetween(0, 8);
	cout << NumberOfSetBitsBetween(3, 8);
	cout << NumberOfSetBitsBetween(4, 8);
	cout << NumberOfSetBitsBetween(7, 8);
	cout << NumberOfSetBitsBetween(8, 8);

	int o; cin >> o;
	return 0;
}