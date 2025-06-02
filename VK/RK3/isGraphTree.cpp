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

    bool isTree() {
        if (numVertices == 0) { // Если нет ребер, значит не дерево
            return false;
        }

        std::vector<bool> visited(numVertices, false); // Вектор посещенных вершины
        std::queue<std::pair<int, int>> q; // Очередь для обхода дерева пара вершина - родитель
        
        q.push({ 0, -1 }); // У корня не должно быть родителей
        visited[0] = true; // Посещаем первую вершину
        int visitedCount = 1;

        while (!q.empty()) { // Пока не прошлись по всем элементам
            int current = q.front().first; // Получаем текущую вершину
            int parent = q.front().second; // И родителя
            q.pop();

            for (int neighbor : adjList[current]) { // Для каждого соседа
                if (!visited[neighbor]) { // Если сосед не посещен
                    visited[neighbor] = true; // Посещаем
                    visitedCount++; // Увеличиваем количество посещений
                    q.push({ neighbor, current }); // И добалвяем в очередь (вершина - родитель)
                }
                else if (neighbor != parent) {  // Если уже был посещен и при этом не является родителем
                   
                    return false; // Значит в графе есть цикл и он не дерево
                }
            }
        }

        
        return (visitedCount == numVertices); // Проверяем, что все вершины посещены
    }
};

void run(std::istream& input, std::ostream& output) {

    int N, M;
    input >> N >> M;

    if (M != N - 1) { // Если количество рёбер не равно N - 1, это точно не дерево
        
        output << 0;
        return;;
    }

    Graph graph(N);

    for (int i = 0; i < M; i++) {
        int u, v;
        input >> u >> v;
        graph.addEdge(u, v);
    }

    output << (graph.isTree() ? 1 : 0);

}

int main() {
    run(std::cin, std::cout);
    return 0;
}
