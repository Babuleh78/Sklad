#include <iostream>
#include <iomanip>
#include <algorithm>
#include <unordered_map>    
#include <string>
int main() {
    std::string s;
    std::cin >> s;


    std::unordered_map<char, int> hash;

    for (char symb : s){
        hash[symb]++;
    }

    int n = s.size();
    int ans = 0;


    for (auto& it : hash){
        ans += it.second * (-it.second + n);
    }

    std::cout << ans/2 +1;
    return 0;
}
