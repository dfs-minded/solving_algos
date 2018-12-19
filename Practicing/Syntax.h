#pragma once

// Free template function

template <typename T>
void Print(const vector<T>& data) {
	for (auto& d : data)
		cout << d << " ";

	cout << endl;
}

template<class InputIt>
void Print(InputIt first, InputIt last) {
	while (first != last) {
		cout << *first << " ";
		++first;
	}

	cout << endl;
}

int main()
{
	// 1. IOTA

	vector<int> autofilled(10);
	iota(autofilled.begin(), autofilled.end(), -5);
	//Print<int>(autofilled);

	// 2. Reverse iterators

	//Print(autofilled.begin(), autofilled.end());
	//Print(autofilled.rbegin(), autofilled.rend());

	char t = static_cast<char>(48);
	char t2 = static_cast<char>(1);

	cin.get();
	return 0;
}


