#include <iostream>
#include <vector>

int main() {
    int n, m;
    std::cin >> n >> m;

    std::vector<std::vector<char>> field(n, std::vector<char>(m));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            std::cin >> field[i][j];
        }
    }

    std::vector<std::pair<int, int>> directions = {
        {0, 1},   // горизонталь 
        {1, 0},   // вертикаль 
        {1, 1},   // диагональ 
        {1, -1}   // диагональ 
    };

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            char c = field[i][j];
            if (c != 'X' && c != 'O') continue; 

            for (const auto& dir : directions) {
                int dx = dir.first;
                int dy = dir.second;

                int end_i = i + 4 * dx;
                int end_j = j + 4 * dy;

                if (end_i < 0 || end_i >= n || end_j < 0 || end_j >= m) {
                    continue;
                }

                bool found = true;
                for (int step = 1; step < 5; step++) {
                    if (field[i + step * dx][j + step * dy] != c) {
                        found = false;
                        break;
                    }
                }

                if (found) {
                    std::cout << "YES";
                    return 0;
                }
            }
        }
    }

    std::cout << "NO";
    return 0;
}
