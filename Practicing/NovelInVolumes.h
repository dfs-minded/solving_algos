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

bool CanSplit(const vector<int>& pages_num, int volumes_left, int max_pages) {
	int i = 0;
	int curr_sum = 0;

	for (; i < pages_num.size() && volumes_left; ++i) {
		if (pages_num[i] > max_pages) return false;

		if (curr_sum + pages_num[i] > max_pages) {
			--volumes_left;
			curr_sum = pages_num[i];
		} else
			curr_sum += pages_num[i];
	}
	
	return (volumes_left > 0) && (i >= pages_num.size());
}

int SolveNovelInVolumes(const vector<int>& pages_num, int vols_num) {
	int lo = 0;
	int hi = accumulate(pages_num.begin(), pages_num.end(), 0);

	while (hi - lo > 1) {
		int mid = lo + (hi - lo) / 2;
		if (CanSplit(pages_num, vols_num, mid)) hi = mid;
		else lo = mid;
	}

	return hi;
}

int main() {
	vector<int> pages_num;
	int vols_num;

	tie(pages_num, vols_num) = Read();
	Write(SolveNovelInVolumes(pages_num, vols_num));

	//int o; cin >> o;
	return 0;
}

