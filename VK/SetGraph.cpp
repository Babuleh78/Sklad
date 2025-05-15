#include <vector>
#include <iostream>
#include <unordered_set>
#include <algorithm>
#include <stdexcept>
#include "IGraph.h"

class SetGraph : public IGraph {
private:
    std::vector<std::unordered_set<int>> adjacencySets; // Множество смежностей

    bool isValidIndex(int index) const { // Проверка индекса вершины на корректность 
        return index >= 0 && index < VerticesCount();
    }

public:
    explicit SetGraph(int vertexCount) : adjacencySets(vertexCount) {} // Конструктор создает граф с vertexCount вершинами

    explicit SetGraph(const IGraph& other) { // Конструктор копирования из другого графа
        int size = other.VerticesCount();
        adjacencySets.resize(size);

        for (int from = 0; from < size; from++) {
            for (int to : other.GetNextVertices(from)) {
                adjacencySets[from].insert(to);
            }
        }
    }

    void AddEdge(int from, int to) override { // Добавляет ребро
        if (!isValidIndex(from) || !isValidIndex(to)) {
            throw std::out_of_range("Invalid index");
        }
        adjacencySets[from].insert(to);
    }

    int VerticesCount() const override { // Возвращает количество вершин
        return adjacencySets.size();
    }

    std::vector<int> GetNextVertices(int vertex) const override { // Получение смежных вершин
        if (!isValidIndex(vertex)) {
            throw std::out_of_range("Invalid index");
        }

        return std::vector<int>(adjacencySets[vertex].begin(), adjacencySets[vertex].end()); 
    }

    std::vector<int> GetPrevVertices(int vertex) const override { // Получение всех вершин, из которых есть ребро в заданную
        if (!isValidIndex(vertex)) {
            throw std::out_of_range("Invalid  index");
        }

        std::vector<int> prevVertices;
        for (int from = 0; from < VerticesCount(); from++) { // Проходимся по каждому ребру
            if (adjacencySets[from].count(vertex) > 0) { // Если целевая вершина содержится в множестве
                prevVertices.push_back(from); // Добавляем ее
            }
        }
        return prevVertices;
    }
};

void runSetTests(std::istream& input, std::ostream& output) {
    // Тест 1: Создание графа
    {
        SetGraph graph(5);
        if (graph.VerticesCount() != 5) {
            output << "Test 1 FAILED: Wrong vertices count\n";
        }
        else {
            output << "Test 1 PASSED: Correct vertices count\n";
        }
    }

    // Тест 2: Добавление ребер
    {
        SetGraph graph(3);
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

    // Тест 3: Предыдущие вершины
    {
        SetGraph graph(4);
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

    // Тест 4: Автоматическая уникальность ребер
    {
        SetGraph graph(2);
        graph.AddEdge(0, 1);
        graph.AddEdge(0, 1); // Дубликат

        if (graph.GetNextVertices(0).size() != 1) {
            output << "Test 4 FAILED: Duplicate edge problem\n";
        }
        else {
            output << "Test 4 PASSED: Duplicate edges handled\n";
        }
    }

    // Тест 5: Неверные индексы
    {
        SetGraph graph(2);
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
        SetGraph graph1(3);
        graph1.AddEdge(0, 1);
        graph1.AddEdge(1, 2);

        SetGraph graph2(graph1);

        if (graph2.GetNextVertices(0) != std::vector<int>{1}) {
            output << "Test 6 FAILED: Copy constructor problem\n";
        }
        else {
            output << "Test 6 PASSED: Graph copied correctly\n";
        }
    }

    // Тест 7: Изолированная вершина
    {
        SetGraph graph(3);
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

    // Тест 8: Проверка наличия ребра
    {
        SetGraph graph(3);
        graph.AddEdge(0, 1);

        // Проверяем косвенно через GetNextVertices
        auto next = graph.GetNextVertices(0);
        if (std::find(next.begin(), next.end(), 1) == next.end()) {
            output << "Test 8 FAILED: Edge not found\n";
        }
        else {
            output << "Test 8 PASSED: Edge exists\n";
        }
    }

    // Тест 9: Большой граф
    {
        const int SIZE = 100;
        SetGraph graph(SIZE);

        for (int i = 0; i < SIZE; ++i) {
            for (int j = 0; j < SIZE; ++j) {
                if ((i + j) % 3 == 0) {
                    graph.AddEdge(i, j);
                }
            }
        }

        int edgeCount = 0;
        for (int i = 0; i < SIZE; ++i) {
            edgeCount += graph.GetNextVertices(i).size();
        }

        if (edgeCount != SIZE * SIZE / 3 + (SIZE % 3 ? 1 : 0)) {
            output << "Test 9 FAILED: Large graph problem\n";
        }
        else {
            output << "Test 9 PASSED: Large graph handled\n";
        }
    }

    // Тест 10: Производительность вставки
    {
        const int SIZE = 1000;
        SetGraph graph(SIZE);

        for (int i = 0; i < SIZE; ++i) {
            graph.AddEdge(i, (i + 1) % SIZE);
        }

        if (graph.GetNextVertices(0).size() != 1) {
            output << "Test 10 FAILED: Insertion performance problem\n";
        }
        else {
            output << "Test 10 PASSED: Insertion performance OK\n";
        }
    }
}




int TestSetGraph() {
    runSetTests(std::cin, std::cout);
    return 0;
}
