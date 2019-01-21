#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <iomanip>

using namespace std;

void Read(vector<double>& first, vector<double>& second) {
	ifstream input;
	input.open("input.txt");

	if (input.is_open()) {
		int N; input >> N;
		first.resize(N);
		second.resize(N);

		for (int i = 0; i < N; ++i) {
			input >> first[i];
			input >> second[i];
		}

		input.close();
	}
}

void Write(double res) {
	ofstream output("output.txt");
	if (output.is_open()) {
		output << std::fixed << std::setprecision(2) << res;
		output.close();
	}
}

double Solve(const vector<double>& uds_rate, const vector<double>& eur_rate) {
	double kInitialCapital = 100;
	// max_profit_can_earn[0] - max sum in RUB can get at day i
	// max_profit_can_earn[1] - max amount of USD can get at day i
	// max_profit_can_earn[2] - max amount of EUR can get at day i
	vector<vector<double>> max_profit_can_earn(uds_rate.size(), vector<double>(3));
	max_profit_can_earn[0][0] = kInitialCapital;
	max_profit_can_earn[0][1] = kInitialCapital / uds_rate[0];
	max_profit_can_earn[0][2] = kInitialCapital / eur_rate[0];

	for (int day = 1; day < uds_rate.size(); ++day) {
		max_profit_can_earn[day][0] = max({ max_profit_can_earn[day - 1][0], 
							uds_rate[day] * max_profit_can_earn[day - 1][1],  
							eur_rate[day] * max_profit_can_earn[day - 1][2] });

		max_profit_can_earn[day][1] = max({ max_profit_can_earn[day - 1][1], // usd from prev day
					max_profit_can_earn[day - 1][0] / uds_rate[day], // buy usd today using rub from prev day
					(eur_rate[day] * max_profit_can_earn[day - 1][2]) / uds_rate[day] }); // sell eur from prev day, buy usd
		
		max_profit_can_earn[day][2] = max({ max_profit_can_earn[day - 1][2], // eur from prev day
					max_profit_can_earn[day - 1][0] / eur_rate[day], // buy eur today using rub from prev day
					(uds_rate[day] * max_profit_can_earn[day - 1][1]) / eur_rate[day] }); // sell usd, buy eur 
	}
	
	return max_profit_can_earn.back()[0];
}

int main() {
	vector<double> first, second;
	Read(first, second);
	Write(Solve(first, second));
	return 0;
}
