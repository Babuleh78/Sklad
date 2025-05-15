#include <vector>
#include <utility>
#include <iostream>
#include <stdexcept>
#include <algorithm>
#include "IGraph.h"

class ArcGraph : public IGraph {
private:
    std::vector<std::pair<int, int>> edges; // Массив пар {from, to}
    int vertexCount; // Количество вершин

    bool isValidIndex(int index) const { // Проверка индекса вершины на корректность 
        return index >= 0 && index < vertexCount;
    }

public:
    explicit ArcGraph(int vertexCount) : vertexCount(vertexCount) {} // Конструктор создает граф с vertexCount вершинами

    explicit ArcGraph(const IGraph& other) : vertexCount(other.VerticesCount()) { // Конструктор копирования из другого графа
        for (int from = 0; from < vertexCount; ++from) { // Заполнение edges всеми доступными from -> to
            for (int to : other.GetNextVertices(from)) {
                edges.emplace_back(from, to);
            }
        }
    }

    void AddEdge(int from, int to) override { // Добавляет ребро
        if (!isValidIndex(from) || !isValidIndex(to)) {
            throw std::out_of_range("Invalid index");
        }
        edges.emplace_back(from, to);
    }

    int VerticesCount() const override { // Возвращает количество вершин
        return vertexCount;
    }

    std::vector<int> GetNextVertices(int vertex) const override { // Получение смежных вершин
        if (!isValidIndex(vertex)) {
            throw std::out_of_range("Invalid index");
        }

        std::vector<int> nextVertices;
        for (const auto& edge : edges) { // Находим все пары
            if (edge.first == vertex) {
                nextVertices.push_back(edge.second);
            }
        }
        return nextVertices;
    }

    std::vector<int> GetPrevVertices(int vertex) const override { // Получение всех вершин, из которых есть ребро в заданную
        if (!isValidIndex(vertex)) {
            throw std::out_of_range("Invalid index");
        }

        std::vector<int> prevVertices;
        for (const auto& edge : edges) { // Обратный алгоритм
            if (edge.second == vertex) {
                prevVertices.push_back(edge.first);
            }
        }
        return prevVertices;
    }
};

void runArcTests(std::istream& input, std::ostream& output) {
    // Тест 1: Создание графа с заданным количеством вершин
    {
        ArcGraph graph(5);
        if (graph.VerticesCount() != 5) {
            output << "Test 1 FAILED: Wrong vertices count\n";
        }
        else {
            output << "Test 1 PASSED: Correct vertices count\n";
        }
    }

    // Тест 2: Добавление и проверка рёбер
    {
        ArcGraph graph(3);
        graph.AddEdge(0, 1);
        graph.AddEdge(0, 2);

        auto next = graph.GetNextVertices(0);
        std::sort(next.begin(), next.end());
        if (next != std::vector<int>{1, 2}) {
            output << "Test 2 FAILED: Wrong next vertices\n";
        }
        else {
            output << "Test 2 PASSED: Correct edge addition\n";
        }
    }

    // Тест 3: Проверка предыдущих вершин
    {
        ArcGraph graph(4);
        graph.AddEdge(0, 3);
        graph.AddEdge(1, 3);
        graph.AddEdge(2, 3);

        auto prev = graph.GetPrevVertices(3);
        std::sort(prev.begin(), prev.end());
        if (prev != std::vector<int>{0, 1, 2}) {
            output << "Test 3 FAILED: Wrong previous vertices\n";
        }
        else {
            output << "Test 3 PASSED: Correct previous vertices\n";
        }
    }

    // Тест 4: Дублирование рёбер
    {
        ArcGraph graph(2);
        graph.AddEdge(0, 1);
        graph.AddEdge(0, 1); // Дубликат

        if (graph.GetNextVertices(0).size() != 2) {
            output << "Test 4 FAILED: Duplicate edges problem\n";
        }
        else {
            output << "Test 4 PASSED: Duplicate edges handled\n";
        }
    }

    // Тест 5: Обработка неверных индексов
    {
        ArcGraph graph(2);
        bool exceptionThrown = false;
        try {
            graph.AddEdge(0, 5);
        }
        catch (const std::out_of_range&) {
            exceptionThrown = true;
        }

        if (!exceptionThrown) {
            output << "Test 5 FAILED: No exception for invalid index\n";
        }
        else {
            output << "Test 5 PASSED: Exception thrown for invalid index\n";
        }
    }

    // Тест 6: Копирование графа
    {
        ArcGraph graph1(3);
        graph1.AddEdge(0, 1);
        graph1.AddEdge(1, 2);

        ArcGraph graph2(graph1);

        if (graph2.GetNextVertices(0) != std::vector<int>{1}) {
            output << "Test 6 FAILED: Copy constructor problem\n";
        }
        else {
            output << "Test 6 PASSED: Graph copied correctly\n";
        }
    }

    // Тест 7: Изолированная вершина
    {
        ArcGraph graph(3);
        graph.AddEdge(0, 1);
        graph.AddEdge(1, 2);

        auto next = graph.GetNextVertices(2);
        if (!next.empty()) {
            output << "Test 7 FAILED: Isolated vertex problem\n";
        }
        else {
            output << "Test 7 PASSED: Isolated vertex handled\n";
        }
    }

    // Тест 8: Петли (ребро в себя)
    {
        ArcGraph graph(2);
        graph.AddEdge(0, 0);

        auto next = graph.GetNextVertices(0);
        if (next != std::vector<int>{0}) {
            output << "Test 8 FAILED: Loop edge problem\n";
        }
        else {
            output << "Test 8 PASSED: Loop edge handled\n";
        }
    }

    // Тест 9: Большой граф
    {
        const int SIZE = 100;
        ArcGraph graph(SIZE);

        for (int i = 0; i < SIZE; ++i) {
            graph.AddEdge(i, (i + 1) % SIZE);
        }

        if (graph.GetNextVertices(SIZE - 1).size() != 1 ||
            graph.GetNextVertices(SIZE - 1)[0] != 0) {
            output << "Test 9 FAILED: Large graph problem\n";
        }
        else {
            output << "Test 9 PASSED: Large graph handled\n";
        }
    }

    // Тест 10: Производительность вставки
    {
        const int SIZE = 1000;
        ArcGraph graph(SIZE);

        for (int i = 0; i < SIZE; ++i) {
            for (int j = 0; j < SIZE; ++j) {
                if ((i + j) % 5 == 0) {
                    graph.AddEdge(i, j);
                }
            }
        }

        int edgeCount = 0;
        for (int i = 0; i < SIZE; ++i) {
            edgeCount += graph.GetNextVertices(i).size();
        }

        if (edgeCount != SIZE * (SIZE / 5 + (SIZE % 5 != 0 ? 1 : 0))) {
            output << "Test 10 FAILED: Insertion performance problem\n";
        }
        else {
            output << "Test 10 PASSED: Insertion performance OK\n";
        }
    }
}

int TestArcGraph() {
    runArcTests(std::cin, std::cout);
    return 0;
}
