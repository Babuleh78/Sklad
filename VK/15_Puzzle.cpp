#include <iostream>
#include <set>
#include <map>
#include <string>
#include <vector>

//Написать алгоритм для решения игры в “пятнашки”.Решением задачи является приведение к виду :
//[1 2 3 4] [5 6 7 8] [9 10 11 12] [13 14 15 0] , где 0 задает 
//пустую ячейку.Достаточно найти хотя бы какое - то решение.
//Число перемещений костяшек не обязано быть минимальным.





class Board {
public:
    Board(std::vector<int>& g) {
        int j = 1;
        for (auto i : g) {
            Start.push_back(i);
            Final.push_back(j);
            j++;
        }
        Final[15] = 0;
    }

    bool isSolutionExists() {
        int inv = 0;
        for (int i = 0; i < 16; ++i)
            if (Start[i])
                for (int j = 0; j < i; ++j)
                    if (Start[j] > Start[i])
                        ++inv;
        for (int i = 0; i < 16; ++i)
            if (Start[i] == 0)
                inv += 1 + i / 4;
        return (inv & 1) == 0;
    }

    char WhichMove(std::vector<int> from, std::vector<int> to) {
        int FirstSpacePosition = 0;
        int SecSpacePosition = 0;
        for (int i = 0; i < 16; i++) {
            if (from[i] == 0) {
                FirstSpacePosition = i;
                break;
            }
        }
        for (int i = 0; i < 16; i++) {
            if (to[i] == 0) {
                SecSpacePosition = i;
                break;
            }
        }

        switch (SecSpacePosition - FirstSpacePosition) {
            case 1: return 'R';
            case -1: return 'L';
            case 4: return 'D';
            case -4: return 'U';
            default: return 0;
        }
    }

    void GetNextVert(std::vector<int>& v, std::vector<std::vector<int>>& next) {
        int SpacePosition = 0;
        for (int i = 0; i < 16; i++) {
            if (v[i] == 0) {
                SpacePosition = i;
                break;
            }
        }
        int i = 0;
        if (SpacePosition < 15 && SpacePosition % 4 != 3) {
            next.push_back(v);
            std::swap(next[i][SpacePosition], next[i][SpacePosition + 1]);
            i++;
        }

        if (SpacePosition > 0 && SpacePosition % 4 != 0) {
            next.push_back(v);
            std::swap(next[i][SpacePosition], next[i][SpacePosition - 1]);
            i++;
        }

        if (SpacePosition >= 4) {
            next.push_back(v);
            std::swap(next[i][SpacePosition], next[i][SpacePosition - 4]);
            i++;
        }

        if (SpacePosition <= 11) {
            next.push_back(v);
            std::swap(next[i][SpacePosition], next[i][SpacePosition + 4]);
        }
    }

    int CountTheHeuristic(std::vector<int> v) {
        int Heuristic = 0;
        for (int i = 0; i < v.size(); i++) {
            int elem = v[i];
            if (elem == 0) {
                Heuristic += abs(3 - i / 4) + abs(3 - i % 4);
            }
            else {
                Heuristic += abs((elem - 1) / 4 - i / 4) + abs((elem - 1) % 4 - i % 4);
            }
        }
        return Heuristic;
    }

    const std::vector<int>& getStart() const { return Start; }
    const std::vector<int>& getFinal() const { return Final; }

private:
    std::vector<int> Start;
    std::vector<int> Final;
};


std::string SolvePuzzle(Board& board) {

   
    if (!board.isSolutionExists()) {
        return "-1";
    }

    std::set<std::pair<int, std::vector<int>>> q;
    std::set<std::vector<int>> used;
    std::map<std::vector<int>, int> g;
    std::map<std::vector<int>, int> f;
    std::map<std::vector<int>, std::vector<int>> parents;

    std::vector<int> Start = board.getStart();
    std::vector<int> Final = board.getFinal();

    g[Start] = 0;
    f[Start] = g[Start] + board.CountTheHeuristic(Start);
    q.insert({ f[Start], Start });

    while (!q.empty()) {
        std::vector<int> tmp = q.begin()->second;
        q.erase(q.begin());

        if (tmp == Final) {
            std::string answer = "";
            int NumOfMoves = 0;
            std::string ReverseWay;
            std::vector<int> tmp = Final;
            std::vector<int> g = std::vector<int>(16, 0);
            Board tempBoard(g);

            while (tmp != Start) {
                NumOfMoves++;
                std::vector<int> prev = parents.at(tmp);
                ReverseWay += tempBoard.WhichMove(tmp, prev);
                tmp = prev;
            }

            answer += std::to_string(NumOfMoves) + "\n";
            for (int i = (int)ReverseWay.length() - 1; i >= 0; i--) {
                answer+= ReverseWay[i];
            }
            return answer;
            
        }

        used.insert(tmp);
        std::vector<std::vector<int>> NextVert;
        board.GetNextVert(tmp, NextVert);

        for (auto v : NextVert) {
            int TentScore = g[tmp] + 1;
            if (used.find(v) != used.end() && TentScore >= g[v]) {
                continue;
            }
            if (used.find(v) == used.end() || TentScore < g[v]) {
                parents[v] = tmp;
                g[v] = TentScore;
                f[v] = g[v] + board.CountTheHeuristic(v);
                if (q.find({ f[v] - g[v], v }) == q.end())
                    q.insert({ f[v] - g[v], v });
            }
        }
    }
}

void run(std::istream& input, std::ostream& output) {

    std::vector<int> g;
    for (int i = 0; i < 16; i++) {
        int tmp = 0;
        std::cin >> tmp;
        g.push_back(tmp);
    }
    Board board(g);
    output << SolvePuzzle(board);
}


int main() {

    run(std::cin, std::cout);
    return 0;
   
}
