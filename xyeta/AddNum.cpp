#include <iostream>
#include <string>
#include <vector>

using namespace std;


int main() {
	setlocale(LC_ALL, "RU");

	int num[3] = { 5, 9, 6 };
	string s;
	int g;

	vector<int> arr2;
	vector<int> arr5;

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			for (int k = 0; k < 3; k++) {
				s = to_string(num[i]) + to_string(num[j]) + to_string(num[k]);
				g = stoi(s);
				if (g % 2 == 0)
					arr2.push_back(g);
				if (g % 5 == 0)
					arr5.push_back(g);
			}
		}
	}

	for (const auto& element : arr2) {
		std::cout << element << " ";
	}
	cout << endl;
	for (const auto& element : arr5) {
		std::cout << element << " ";
	}


	return 0;
}