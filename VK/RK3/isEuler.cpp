#include <iostream>
#include <vector>
#include <queue>

//Проверить, является ли неориентированный граф деревом.
//Граф должен быть реализован в виде класса.

class Graph { // Класс графа
private:
    int numVertices; // Количество вершин
    std::vector<std::vector<int>> adjList; // Матрица смежности

public:
    Graph(int n) : numVertices(n), adjList(n) {} // Конструктор

    void addEdge(int u, int v) { // Добавление вершины в неориентированный граф
        adjList[u].push_back(v);
        adjList[v].push_back(u);
    }

    bool isConnected() { // Проверяем, является ли граф связным
        if (numVertices == 0) {
            return true;
        }

        std::vector<bool> visited(numVertices, false); // Вектор посещенных вершины
        std::queue<int> q; // Очередь для обход

        q.push(0);
        visited[0] = true;
        int visitedCount = 1;

        while (!q.empty()) { // Пока не прошлись по всем вершинам
            int current = q.front(); // Получаем текущую вершину
            q.pop();

            for (int neighbor : adjList[current]) { // Для каждого соседа
                if (!visited[neighbor]) { // Если вершина не посещена
                    q.push(neighbor);
                    visitedCount++;
                    visited[neighbor] = true;
                }
            }

        }

        return (visitedCount == numVertices); // Если удалось обойти все вершины
    }

    bool isEuler() {
        if (!isConnected()) {
            return false;
        }

        for (int i = 0; i < numVertices; i++) {
            if (adjList[i].size() % 2 != 0) { // У эйлерова графа все вершины должны иметь чётную степень
                return false;
            }
        }

        return true;
    }
};

void run(std::istream& input, std::ostream& output) {

    int N, M;
    input >> N >> M;

    Graph graph(N);

    for (int i = 0; i < M; ++i) {
        int u, v;
        input >> u >> v;
        graph.addEdge(u, v);
    }

    output << (graph.isEuler() ? 1 : 0);

}

int main() {
    run(std::cin, std::cout);
    return 0;
}
