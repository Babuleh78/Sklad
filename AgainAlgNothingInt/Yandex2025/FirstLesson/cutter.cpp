#include <iostream>
#include <vector>
#include <unordered_map>
#include <queue>
int main() {
    int n, m;
    std::cin >> n >> m;

    std::string s;
    std::cin >> s;

    std::vector<std::string> rs;
    int len = n/m;

    std::unordered_map<std::string, std::queue<int>> pieceToIndices;

    for (int i = 0; i < m; i++) {
        std::string piece;
        std::cin >> piece;
        pieceToIndices[piece].push(i + 1);
    }

    std::vector<int> res;
    for (int i =0; i<m; i++){
        std::string part = s.substr(i * len, len);

        int idx = pieceToIndices[part].front();
        pieceToIndices[part].pop();

        res.push_back(idx);
    }

    for (int i = 0; i < m; ++i) {
        if (i > 0) std::cout << " ";
        std::cout << res[i];
    }

}
