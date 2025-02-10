#include <iostream>
#include <string>
#include <vector>

int main()
{
	long long k, m, d;
	std::cin >> k;
	std::cin >> m;
	std::cin >> d;

	int happy_days = 0;
	int current_read = 0;
	while (1) {
		if (5 * k - (current_read * k + 28) >= 0) {
			happy_days += 7;
			current_read += 7;
		}
		else {
			break;
		}
	}
	while (m >= 0) {
		current_read++;
		if (d < 6) m += k;
		if (m < current_read) {
			break;
		}
		d = (d % 7) + 1;
		m -= current_read;

		happy_days++;
	}



	std::cout << happy_days;
	return 0;
}
