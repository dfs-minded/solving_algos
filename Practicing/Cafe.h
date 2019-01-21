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

tuple<int, int, int> Solve(vector<int> prices) {
	const int kMinPromotionPrice = 101;
	int N = prices.size();
	vector<long long> min_price_can_get_curr(N + 1, numeric_limits<int>::max());
	auto min_price_can_get_prev = min_price_can_get_curr;
	vector<int> coupons_used(N + 1);

	min_price_can_get_prev[0] = prices[0];

	if (prices[0] >= kMinPromotionPrice)
		min_price_can_get_prev[1] = prices[0];

	for (int day = 1; day < N; ++day) {
		for (int coupons = 0; coupons <= N; ++coupons) {
			// buy today
			auto curr_price = min_price_can_get_prev[coupons] + prices[day];
			min_price_can_get_curr[coupons] = min(min_price_can_get_curr[coupons], curr_price);

			// if we can and we get better price, use coupon from prev day
			if (coupons < N && min_price_can_get_prev[coupons + 1] < min_price_can_get_curr[coupons]) {
				min_price_can_get_curr[coupons] = min_price_can_get_prev[coupons + 1];
				coupons_used[coupons] = coupons_used[coupons + 1] + 1;
			}

			if (prices[day] >= kMinPromotionPrice && coupons > 0) {
				auto price = min_price_can_get_prev[coupons - 1] + prices[day];
				if (price < min_price_can_get_curr[coupons]) {
					min_price_can_get_curr[coupons] = price;
					coupons_used[coupons] = coupons_used[coupons - 1] + 1;
				}
			}
		}

		swap(min_price_can_get_curr, min_price_can_get_prev);
		min_price_can_get_curr = vector<long long>(N + 1, numeric_limits<int>::max());
	}

	auto min_price = min_price_can_get_prev[0];
	int coupons_left = 0;
	for (int i = 1; i <= N; ++i) {
		if (min_price_can_get_prev[i] < min_price) {
			coupons_left = i;
			min_price = min_price_can_get_prev[i];
		}
	}

	return { min_price_can_get_prev[coupons_left], coupons_left, coupons_used[coupons_left] };
}

int main() {
	vector<int> data;
	int N;

	tie(data, N) = Read();
	Write(Solve(data));

	return 0;
}