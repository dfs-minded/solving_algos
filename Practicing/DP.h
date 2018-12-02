#pragma once

int PlaceRestaurants(const vector<int>& distances, const vector<int>& profit, int min_dist) {
	if (distances.empty()) return 0;
	vector<int> max_can_earn = profit; // max amount we can earn placing a restaurant at a particular location
	int left = 0; // last index, where located a restaurant with distance > min_dist
	int best_profit_sofar = max_can_earn[0]; // max can earn from restaurants on the allowed distance
	int curr_dist = distances[0];

	for (int i = 1; i < distances.size(); ++i) {
		while (i < distances.size() && curr_dist < min_dist) {
			curr_dist += distances[i++];
		} 
		max_can_earn[i] += best_profit_sofar;
		best_profit_sofar = max_can_earn[i];
		curr_dist -= distances[left++];
	}

	return *max_element(max_can_earn.begin(), max_can_earn.end());
}