#include <iostream>
#include <vector>
#include <queue>

//Требуется отыскать самый выгодный маршрут между городами.
//Требования: время работы O((N + M)logN), где N - количество городов, M - известных дорог между ними.
//Формат входных данных.
//Первая строка содержит число N – количество городов.
//Вторая строка содержит число M - количество дорог.
//Каждая следующая строка содержит описание дороги(откуда, куда, время в пути).
//Последняя строка содержит маршрут(откуда и куда нужно доехать).
//Формат выходных данных.
//Вывести длину самого выгодного маршрута.

struct Edge { // Вершина взвешенного графа
	int to; // Пункт отправления
	int weight; // Пункт назначения
	Edge(int t, int w) : to(t), weight(w) {}
};

struct IGraph {
	virtual ~IGraph() {}

	// Добавление ребра от from к to.
	virtual void AddEdge(int from, int to, int weight) = 0;

	virtual int VerticesCount() const = 0;

	virtual std::vector<Edge> GetNextEdges(int vertex) const = 0; // Для ориентированного графа имеет смысл только данный метод
};



class ListGraph :IGraph {

private:
	std::vector <std::vector<Edge>> adjacencyLists; // Массив списков смежности

	bool isValidIndex(int index) const { // Проверка индекса вершины на корректность 
		return index >= 0 && index < VerticesCount();
	}

public:

	explicit ListGraph(int vertexCount) : adjacencyLists(vertexCount) {} // Конструктор создает граф с vertexCount вершинами

	explicit ListGraph(const IGraph& other) {// Конструктор копирования из другого графа
		adjacencyLists.resize(other.VerticesCount()); // Изменение размера списка смежности
		for (int i = 0; i < other.VerticesCount(); i++) {
			adjacencyLists[i] = other.GetNextEdges(i); // Добавляем для каждого индекса все смежные вершины
		}
	}

	int VerticesCount() const override { // Возвращает количество вершин
		return adjacencyLists.size();
	}


	void AddEdge(int from, int to, int weight) override { // Добавляет ребро
		if (!isValidIndex(from) || !isValidIndex(to)) {
			throw std::out_of_range("Invalid index");
		}
		adjacencyLists[from].emplace_back(to, weight);
	}

	std::vector<Edge> GetNextEdges(int vertex) const override {
		if (!isValidIndex(vertex)) {
			throw std::out_of_range("Invalid index");
		}
		return adjacencyLists[vertex];
	}
};

int solver(const ListGraph& graph, int start, int end) {

	int vertices = graph.VerticesCount();
	std::vector<int> distances(vertices, INT_MAX);
	distances[start] = 0;

	std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, std::greater<std::pair<int, int>>> pq;
	pq.push({ 0, start });

	while (!pq.empty()) {
		int current_distance = pq.top().first;
		int current_vertex = pq.top().second;
		pq.pop();

		if (current_distance > distances[current_vertex]) {
			continue;
		}

		for (const Edge& edge : graph.GetNextEdges(current_vertex)) {
			int neighbor = edge.to;
			int weight = edge.weight;
			int distance = current_distance + weight;

			if (distance < distances[neighbor]) {
				distances[neighbor] = distance;
				pq.push({ distance, neighbor });
			}
		}
	}

	return distances[end];

}


void run(std::istream& input, std::ostream& output) {
	int N, M;
	input >> N >> M;

	ListGraph graph(N);

	for (int i = 0; i < M; ++i) {
		int from, to, weight;
		input >> from >> to >> weight;
		graph.AddEdge(from, to, weight);
		graph.AddEdge(to, from, weight); // если граф неориентированный
	}

	int start, end;
	input >> start >> end;

	int shortest_path = solver(graph, start, end);
	output << shortest_path;
}


int main() {
    run(std::cin, std::cout);
    return 0;
}
