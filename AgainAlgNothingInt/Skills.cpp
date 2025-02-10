#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;
int main()
{
	int size;
	cin >> size;
	vector<int> dev_skills(size);
	for (int i = 0; i < size; i++) {
		cin >> dev_skills[i];
	}
	vector<int> man_skills(size);
	for (int i = 0; i < size; i++) {
		cin >> man_skills[i];
	}
	int iter;
	cin >> iter;
	for (int i = 0; i < iter; i++) {
		int index, category, buf;
		cin >> index; 
		cin >> category;
		cin >> buf;
		index--;
		if (category == 1) {
			dev_skills[index] += buf;
		}
		else {
			man_skills[index] += buf;
		}

	
		vector<pair<int, int>> delta(size);
		int help_sum = 0;
		for (int j = 0; j < size; j++) {
			pair<int, int> help_p;
			help_p.second = j;
			help_p.first = dev_skills[j] - man_skills[j];
			delta[j] = help_p;
			help_sum += dev_skills[j];
		}
		std::sort(delta.begin(), delta.end());
		int man_value = 0;
		for (int i = 0; i < size / 2; i++) {
			pair<int, int> p = delta[i];
			man_value += man_skills[p.second];
			help_sum -= dev_skills[p.second];
		}
		cout << man_value + help_sum << endl;
		
	}

	return 0;
}
