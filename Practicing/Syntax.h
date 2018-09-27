#pragma once

// Free template function

template <typename T>
void Print(const vector<T>& data) {
	for (auto& d : data)
		cout << d << " ";

	cout << endl;
}


int main()
{
	// 1. IOTA

	vector<int> autofilled(10);
	iota(autofilled.begin(), autofilled.end(), -5);
	Print<int>(autofilled);


	int i; cin >> i;
	return 0;
}


