#include <iostream>
#include<vector>
#include <algorithm>
#include "IGraph.h"
// ListGraph, хранящий граф в виде массива списков смежности




class ListGraph :IGraph {

private:
	std::vector <std::vector<int>> adjacencyLists;

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

	int VerticesCount() const override{ // Возвращает количество вершин
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

void run(std::istream& input, std::ostream& output) { // Тесты
    // Тест 1: Создание графа и проверка количества вершин
    {
        ListGraph graph(5);
        if (graph.VerticesCount() != 5) {
            output << "Test 1 failed: Wrong vertices count\n";
        }
        else {
            output << "Test 1 passed: Vertices count correct\n";
        }
    }

    // Тест 2: Добавление ребер и проверка смежных вершин
    {
        ListGraph graph(4);
        graph.AddEdge(0, 1);
        graph.AddEdge(0, 2);
        graph.AddEdge(1, 3);

        auto next0 = graph.GetNextVertices(0);
        std::sort(next0.begin(), next0.end());
        if (next0 != std::vector<int>{1, 2}) {
            output << "Test 2 failed: Wrong next vertices for vertex 0\n";
        }
        else {
            output << "Test 2 passed: Next vertices correct\n";
        }
    }

    // Тест 3: Проверка предыдущих вершин
    {
        ListGraph graph(4);
        graph.AddEdge(0, 1);
        graph.AddEdge(2, 1);
        graph.AddEdge(3, 1);

        auto prev1 = graph.GetPrevVertices(1);
        std::sort(prev1.begin(), prev1.end());
        if (prev1 != std::vector<int>{0, 2, 3}) {
            output << "Test 3 failed: Wrong previous vertices for vertex 1\n";
        }
        else {
            output << "Test 3 passed: Previous vertices correct\n";
        }
    }

    // Тест 4: Проверка обработки некорректных индексов
    {
        ListGraph graph(3);
        bool exceptionThrown = false;
        try {
            graph.AddEdge(0, 5); // Некорректный индекс
        }
        catch (const std::out_of_range&) {
            exceptionThrown = true;
        }

        if (!exceptionThrown) {
            output << "Test 4 failed: No exception for invalid index\n";
        }
        else {
            output << "Test 4 passed: Exception thrown for invalid index\n";
        }
    }

    // Тест 5: Проверка копирования графа
    {
        ListGraph graph1(3);
        graph1.AddEdge(0, 1);
        graph1.AddEdge(1, 2);
        graph1.AddEdge(2, 0);

        ListGraph graph2(graph1);

        if (graph2.VerticesCount() != 3) {
            output << "Test 5 failed: Wrong vertices count after copy\n";
        }
        else {
            auto next1 = graph2.GetNextVertices(1);
            if (next1 != std::vector<int>{2}) {
                output << "Test 5 failed: Wrong edges after copy\n";
            }
            else {
                output << "Test 5 passed: Graph copied correctly\n";
            }
        }
    } 

    // Тест 6: Пустой граф
    {
        ListGraph graph(0);
        if (graph.VerticesCount() != 0) {
            output << "Test 6 failed: Empty graph has non-zero size\n";
        }
        else {
            output << "Test 6 passed: Empty graph handled correctly\n";
        }
    }

    // Тест 7: Добавление петли
    {
        ListGraph graph(3);
        graph.AddEdge(1, 1); // Петля
        auto next = graph.GetNextVertices(1);
        if (next != std::vector<int>{1}) {
            output << "Test 7 failed: Loop edge not handled correctly\n";
        }
        else {
            output << "Test 7 passed: Loop edge handled correctly\n";
        }
    }

    // Тест 8: Добавление кратных ребер
    {
        ListGraph graph(3);
        graph.AddEdge(0, 1);
        graph.AddEdge(0, 1); // Дублирование ребра
        auto next = graph.GetNextVertices(0);
        if (next.size() != 2 || next[0] != 1 || next[1] != 1) {
            output << "Test 8 failed: Multiple edges not handled correctly\n";
        }
        else {
            output << "Test 8 passed: Multiple edges handled correctly\n";
        }
    }

    // Тест 9: Производительность
    {
        const int SIZE = 1000;
        ListGraph graph(SIZE);

        // Создаем полный граф
        for (int i = 0; i < SIZE; ++i) {
            for (int j = 0; j < SIZE; ++j) {
                if (i != j) graph.AddEdge(i, j);
            }
        }

        bool correct = true;
        for (int i = 0; i < SIZE; ++i) {
            auto next = graph.GetNextVertices(i);
            if (next.size() != SIZE - 1) {
                correct = false;
                break;
            }
        }

        if (!correct) {
            output << "Test 9 failed: Large graph handling problem\n";
        }
        else {
            output << "Test 9 passed: Large graph handled correctly\n";
        }
    }

    // Тест 10: Копирование через присваивание
    {
        ListGraph graph1(3);
        graph1.AddEdge(0, 1);
        graph1.AddEdge(1, 2);

        ListGraph graph2(1);
        graph2 = graph1;

        if (graph2.VerticesCount() != 3) {
            output << "Test 10 failed: Assignment operator vertices count\n";
        }
        else {
            auto next = graph2.GetNextVertices(0);
            if (next != std::vector<int>{1}) {
                output << "Test 10 failed: Assignment operator edges\n";
            }
            else {
                output << "Test 10 passed: Assignment operator works correctly\n";
            }
        }
    }


}

int main() {
	run(std::cin, std::cout);
	return 0;
}
