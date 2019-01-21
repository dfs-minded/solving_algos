#include <iostream>
#include <fstream>
#include <vector>
#include <tuple>
#include <algorithm>
#include <functional>

using namespace std;

pair<vector<int>, int> Read() {
	ifstream input;
	input.open("input.txt");

	vector<int> data;
	int M;

	if (input.is_open()) {
		int N;
		input >> N;
		data.resize(N);

		for (int i = 0; i < N; ++i)
			input >> data[i];

		input >> M;
		input.close();
	}

	return { data, M };
}

void Write(tuple<int, int, int> res) {
	ofstream output("output.txt");
	if (output.is_open())
	{
		output << get<0>(res) << endl;
		output << get<1>(res) << " " << get<2>(res);
		output.close();
	}
}

tuple<int, int, int> Solve(vector<int> values) {
	const int kMinPromotionPrice = 100;
	int N = values.size();
	// days(rows) / coupons left on a specific day (cols)
	vector<vector<long long>> memo(N, vector<long long>(N + 1, numeric_limits<int>::max()));
	memo[0][0] = values[0];
	int max_coupons = 0;
	if (values[0] >= kMinPromotionPrice) {
		memo[0][1] = values[0];
		++max_coupons;
	}

	for (int day = 1; day < N; ++day) {
		if (values[day] >= kMinPromotionPrice) ++max_coupons;
		
		for (int coupons = max_coupons; coupons >= 0; --coupons) {

			// Calc without current coupon, even if we can get it.
			// Min if we do not use a coupon from prev day and if we use.
			memo[day][coupons] = min(memo[day - 1][coupons + 1], values[day] + memo[day - 1][coupons]);

			// if we get new coupon
			if (values[day] >= kMinPromotionPrice && coupons > 0) {
				// Min from what we have already calculated and if we take advantage of todays coupon.
				memo[day][coupons] = min(memo[day][coupons], values[day] + memo[day - 1][coupons - 1]);
			}
		}
	}
	
	auto min_price = memo.back()[0];
	int coupons_left = 0;
	for (int i = 1; i <= max_coupons; ++i) {
		if (memo.back()[i] < min_price) {
			coupons_left = i;
			min_price = memo.back()[i];
		}
	}
	
	return { memo.back()[coupons_left], coupons_left, max_coupons - coupons_left };
}

int main() {
	vector<int> data;
	int N;

	tie(data, N) = Read();
	Write(Solve(data));

	return 0;
}