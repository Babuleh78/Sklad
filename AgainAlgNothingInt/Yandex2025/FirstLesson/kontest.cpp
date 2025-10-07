    #include <iostream>
    #include <iomanip>
    #include <algorithm>
    #include <unordered_map>    
    #include <string>
    #include <unordered_set>
    #include <vector>
    int main() {
        std::unordered_set<int> mySet;
        std::vector<int> uniq;

        int n, k;
        std::cin >> n >> k;
        int bufferTheme = -1;
        for(int i = 0; i<n; i++){
            int value;
            std::cin >> value;

             if(i == 0){
                bufferTheme = value;
            }
            if(mySet.find(value) == mySet.end()){
                mySet.insert(value);
                uniq.push_back(value);
            }
        }

        for (int i = 0; i < k; i++) {
            if (i > 0) std::cout << " ";

            if (i < (int)uniq.size()) {
                std::cout << uniq[i];
            } else {
                std::cout << bufferTheme;
            }
        }

        
        std::cout << '\n';
        return 0;
    }
