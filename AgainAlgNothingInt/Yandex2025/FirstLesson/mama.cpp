#include <iostream>
#include <iomanip>
#include <algorithm>

int main() {
    double a, b, c, v0, v1, v2;
    std::cin >> a >> b >> c >> v0 >> v1 >> v2;

    a = std::min(a, b+c);
    b = std::min(b, a+c);
    c = std::min(c, a+b);

    double t1 = a/v0 + a/v1 + b/v0 + b/v1;
    double t2 = a/v0 + c/v1 +b/v2;
    double t3 = b/v0 + c/v1 + a/v2;


    double ans = std::min({t1, t2, t3});

    std::cout << std::fixed << std::setprecision(12) << ans << std::endl;
    return 0;
}
