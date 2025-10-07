#include <iostream>
#include <vector>

int main() {
    long long n, k;  
    std::cin >> n >> k;

    if (k == 0) {
        std::cout << n << '\n';
        return 0;
    }

    if (n % 10 == 0) {
        std::cout << n << '\n';
        return 0;
    }

    if (n % 10 == 5) {
        if (k >= 1) {
            n += 5;
        }
        std::cout << n << '\n';
        return 0;
    }

    if (n % 10 % 2 == 1) {
        n += n % 10;
        k--;
        if (k == 0) {
            std::cout << n << '\n';
            return 0;
        }
    }

    long long cycles = k / 4;
    n += cycles * 20;
    k %= 4;

    while (k > 0) {
        n += n % 10;
        k--;
    }

    std::cout << n << '\n';
    return 0;
}
