#include <iostream>
#include <fstream>
#include <vector>
#include <tuple>
#include <numeric>

using namespace std;

pair<vector<int>, int> Read() {
	ifstream input;
	input.open("input.txt");
	
	vector<int> pages_num;
	int vols_num;

	if (input.is_open()) {
		int N;
		input >> N;
		pages_num.resize(N);

		for (int i = 0; i < N; ++i)
			input >> pages_num[i];

		input >> vols_num;
		input.close();
	}

	return { pages_num, vols_num };
}

void Write(int res) {
	ofstream output("output.txt");
	if (output.is_open())
	{
		output << res;
		output.close();
	}
}

bool CanSplit(const vector<int>& pages, int volumes, int max_pages_per_vol) {
	int curr_sum = 0;
	int curr_vols = 0;

	for (int i = 0; i < pages.size(); ++i) {
		if (pages[i] > max_pages_per_vol) return false;
		curr_sum += pages[i];

		if (curr_sum > max_pages_per_vol) {
			++curr_vols;
			curr_sum = pages[i];
		}
		else if (i + 1 == pages.size())
			++curr_vols;
	}

	return curr_vols >= volumes;
}

int SolveNovelInVolumes(const vector<int>& pages, int vols_num) {
	int lo = 0;
	int hi = accumulate(pages.begin(), pages.end(), 0);

	while (hi - lo > 1) {
		int mid = lo + (hi - lo) / 2;
		if (CanSplit(pages, vols_num, mid)) hi = mid;
		else lo = mid;
	}

	return hi;
}

int main() {
	vector<int> pages_num; 
	int vols_num;

	tie(pages_num, vols_num) = Read();
	Write(SolveNovelInVolumes(pages_num, vols_num));

	return 0;
}

