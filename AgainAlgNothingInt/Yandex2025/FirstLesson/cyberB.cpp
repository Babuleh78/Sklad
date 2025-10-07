    #include <iostream>
    #include <iomanip>
    #include <algorithm>
    #include <unordered_map>    
    #include <string>
    int main() {
        std::string s;
        std::cin >> s;


        std::unordered_map<char, long long> hash;

        for (char symb : s){
            hash[symb]++;
        }

        long long n = s.size();
        long long ans = 0;


        for (auto& it : hash){
            ans += it.second * (-it.second + n);
        }

        std::cout << ans/2 +1;
        return 0;
    }
