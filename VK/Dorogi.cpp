#include <iostream>
#include <vector>
#include <queue>

//Дан невзвешенный неориентированный граф.В графе может быть несколько кратчайших путей между 
//какими - то вершинами.Найдите количество различных кратчайших путей между заданными вершинами.
//Требования: сложность O(V + E).
//Формат ввода.
//v : кол - во вершин(макс. 50000),
//n : кол - во ребер(макс. 200000),
//n пар реберных вершин,
//пара вершин u, w для запроса.
//Формат вывода.
//Количество кратчайших путей от u к w.

void run(std::istream& input, std::ostream& output) {
    int v, n;
    input >> v >> n; // Получаем количество вершин и ребер

    std::vector<std::vector<int>> adjacencyLists(v); // Создаем вектор, хранящий информацию о смежности (ListGraph)
    for (int i = 0; i < n; i++) { // Заполняем его
        int from, to;
        input >> from >> to;
        // т.к. граф неориентированный двигаться можно в обе стороны
        adjacencyLists[from].push_back(to);
        adjacencyLists[to].push_back(from);
    }

    int u, w; // Получаем начальную и конечную точку
    input >> u >> w;

    std::vector<int> distances(v, -1); // Расстояние от стартовой вершины u до конкретной вершины v, если вершина не посещена, ее значение -1
    std::vector<int> counts(v, 0); // Количество пустей из стартовой вершины u в конкретную вершину v
    std::queue<int> q; // Очередь для BFS

    counts[u] = 1; // Путь из u в u один

    q.push(u); // Добавляем в очередь стартовую вершину

    while (!q.empty()) { // Пока не прошлись по всем вершинам
        int current_vertex = q.front(); 
        q.pop();

        for (int neighbour_vertex : adjacencyLists[current_vertex]) { // Для каждого соседа, куда можно попасть из вершины current_vertex
            if (distances[neighbour_vertex] == -1) { // Вершина не посещена в рамках BFS обхода
                q.push(neighbour_vertex); // Добавляем ее в очередь для последующей обработки

                // Расстояние до соседа от исходной вершины - это расстояние от исходной вершины (u) до текущей (current)  + 1
                distances[neighbour_vertex] = distances[current_vertex] + 1; 
                // Количество путей из соседней вершины равно количеству путей текущей вершины
                counts[neighbour_vertex] = counts[current_vertex];
            }
            else if (distances[neighbour_vertex] == distances[current_vertex] + 1) {  // Если вершина уже посещена и путь кратчайший
                // Значит мы нашли еще один путь и прибавляем к соседу значение текущей вершины
                counts[neighbour_vertex] += counts[current_vertex]; 
            }

        }

    }

    output << counts[w];

    
}

int main() {
    run(std::cin, std::cout);
    return 0;
}

#include <iostream>
#include <vector>
#include <queue>

//Дан невзвешенный неориентированный граф.В графе может быть несколько кратчайших путей между 
//какими - то вершинами.Найдите количество различных кратчайших путей между заданными вершинами.
//Требования: сложность O(V + E).
//Формат ввода.
//v : кол - во вершин(макс. 50000),
//n : кол - во ребер(макс. 200000),
//n пар реберных вершин,
//пара вершин u, w для запроса.
//Формат вывода.
//Количество кратчайших путей от u к w.

struct IGraph {
	virtual ~IGraph() {}

	// Добавление ребра от from к to.
	virtual void AddEdge(int from, int to) = 0;

	virtual int VerticesCount() const = 0;

	virtual std::vector<int> GetNextVertices(int vertex) const = 0;
	virtual std::vector<int> GetPrevVertices(int vertex) const = 0;
};



class ListGraph :IGraph {

private:
	std::vector <std::vector<int>> adjacencyLists; // Массив списков смежности

	bool isValidIndex(int index) const { // Проверка индекса вершины на корректность 
		return index >= 0 && index < VerticesCount();
	}

public:

	explicit ListGraph(int vertexCount) : adjacencyLists(vertexCount) {} // Конструктор создает граф с vertexCount вершинами

	explicit ListGraph(const IGraph& other) {// Конструктор копирования из другого графа
		adjacencyLists.resize(other.VerticesCount()); // Изменение размера списка смежности
		for (int i = 0; i < other.VerticesCount(); i++) {
			adjacencyLists[i] = other.GetNextVertices(i); // Добавляем для каждого индекса все смежные вершины
		}
	}

	int VerticesCount() const override { // Возвращает количество вершин
		return adjacencyLists.size();
	}


	void AddEdge(int from, int to) override { // Добавляет ребро
		if (!isValidIndex(from) || !isValidIndex(to)) {
			throw std::out_of_range("Invalid index");
		}
		adjacencyLists[from].push_back(to);
	}

	std::vector<int> GetNextVertices(int vertex) const override { // Получение смежных вершин
		if (!isValidIndex(vertex)) {
			throw std::out_of_range("Invalid index");
		}
		return adjacencyLists[vertex];
	}

	std::vector<int> GetPrevVertices(int vertex) const override { // Получение всех вершин, из которых есть ребро в заданную
		if (!isValidIndex(vertex)) {
			throw std::out_of_range("Invalid index");
		}

		std::vector<int> PrevVertices;

		for (int from = 0; from < VerticesCount(); from++) { // Проходимся по всем ребрам
			for (int to : adjacencyLists[from]) { // И по всем ребрам, куда можно оттуда добраться
				if (to == vertex) { // Если можем добраться в заданное ребро
					PrevVertices.push_back(from); // Добавляет в список
					break;
				}
			}

		}


		return PrevVertices;

	}
};

int solver(const ListGraph& graph, int u, int w) {

	std::vector<int> distances(graph.VerticesCount(), -1); // Расстояние от стартовой вершины u до конкретной вершины v, если вершина не посещена, ее значение -1
	std::vector<int> counts(graph.VerticesCount(), 0); // Количество пустей из стартовой вершины u в конкретную вершину v
	std::queue<int> q; // Очередь для BFS

	counts[u] = 1; // Путь из u в u один

	q.push(u); // Добавляем в очередь стартовую вершину

	while (!q.empty()) { // Пока не прошлись по всем вершинам
		int current_vertex = q.front();
		q.pop();

		for (int neighbour_vertex : graph.GetNextVertices(current_vertex)) { // Для каждого соседа, куда можно попасть из вершины current_vertex
			if (distances[neighbour_vertex] == -1) { // Вершина не посещена в рамках BFS обхода
				q.push(neighbour_vertex); // Добавляем ее в очередь для последующей обработки

				// Расстояние до соседа от исходной вершины - это расстояние от исходной вершины (u) до текущей (current)  + 1
				distances[neighbour_vertex] = distances[current_vertex] + 1;
				// Количество путей из соседней вершины равно количеству путей текущей вершины
				counts[neighbour_vertex] = counts[current_vertex];
			}
			else if (distances[neighbour_vertex] == distances[current_vertex] + 1) {  // Если вершина уже посещена и путь кратчайший
				// Значит мы нашли еще один путь и прибавляем к соседу значение текущей вершины
				counts[neighbour_vertex] += counts[current_vertex];
			}

		}

	}

	return counts[w];
}


void run(std::istream& input, std::ostream& output) {
    int v, n;
    input >> v >> n; // Получаем количество вершин и ребер

	ListGraph graph(v); // Инициализируем граф

    for (int i = 0; i < n; i++) { // Заполняем его
        int from, to;
        input >> from >> to;
		graph.AddEdge(from, to);
		graph.AddEdge(to, from);
    }

    int u, w; // Получаем начальную и конечную точку
    input >> u >> w;

	output << solver(graph, u, w);
    

    
}

int main() {
    run(std::cin, std::cout);
    return 0;
}
