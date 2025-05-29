#include <iostream>
#include <algorithm>
#include <vector>

//Дан неориентированный связный граф.Требуется найти вес минимального 
//остовного дерева в этом графе с помощью алгоритма Крускала

struct Edge {
    int u; // Начальная вершина ребра
    int v; // Конечная вершина ребра
    int weight; // Вес ребра
    bool operator<(Edge const& _otherEdge) { // Оператор сравнения 
        return weight < _otherEdge.weight;
    }
};


void run(std::istream& input, std::ostream& output) {

    int n, m = 0; // Количество вершин и ребер графа
    input >> n >> m;

    std::vector<Edge> edges(m); // Вектор ребер графа
    for (int i = 0; i < m; i++) {
        input >> edges[i].u >> edges[i].v >> edges[i].weight;
        edges[i].u--; // Чтобы нумерация была с нуля
        edges[i].v--;
    }

    std::sort(edges.begin(), edges.end()); // Сортируем ребра по весу

    std::vector<int> parents(n); // Хранит для каждой вершины ее родителей
    std::vector<int> ranks(n); // Ранги множества для их объединения

    for (int i = 0; i < n; i++) { // Заполняем списки
        parents[i] = i; // Каждая вершина - свое собственное множество
        ranks[i] = 0; // Начальный ранг - 0
    }


    long long total_weight = 0;
    for (const Edge& e : edges) {
        int root_u = e.u; // Поиск корня для вершины u
        while (root_u != parents[root_u]) { // Пока текущий корень не равен его родителю
            parents[root_u] = parents[parents[root_u]]; // Подтягиваем вершину к дедушки (родителю родителя)
            root_u = parents[root_u]; // Переходим к родителю
        }

        int root_v = e.v; // Поиск корня для вершины v 
        while (root_v != parents[root_v]) {
            parents[root_v] = parents[parents[root_v]];
            root_v = parents[root_v];
        }

        if (root_u != root_v) { // Если вершины в разных множествах
            total_weight += e.weight; // Добавляем вес ребра 
           
            if (ranks[root_u] < ranks[root_v]) { // Объединяем множества
                std::swap(root_u, root_v);
            }
            parents[root_v] = root_u; // Обновляем родителя
            if (ranks[root_u] == ranks[root_v]) { // И Ранги
                ranks[root_u]++;
            }
        }
    }

    output << total_weight << "\n";

}


int main() {

    run(std::cin, std::cout);
    return 0;
   
}
