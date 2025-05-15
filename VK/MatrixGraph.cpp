#include <iostream>
#include <vector>
#include <stdexcept>
#include <algorithm>
#include "IGraph.h"

class MatrixGraph : public IGraph {
private:
    std::vector<std::vector<bool>> adjacencyMatrix; // Матрица смежности

    bool isValidIndex(int index) const { // Проверка правильности индекса
        return index >= 0 && index < VerticesCount();
    }

public:
    explicit MatrixGraph(int vertexCount) : adjacencyMatrix(vertexCount, std::vector<bool>(vertexCount, false)) {} // Конструктор по количеству вершин

    explicit MatrixGraph(const IGraph& other) { // Конструктор копирования из другого графа
        int size = other.VerticesCount();
        adjacencyMatrix.resize(size, std::vector<bool>(size, false)); // Приводим к нужному размеру

        for (int from = 0; from < size; from++) { // Заполняем, проходясь по каждому потенциальному from to
            for (int to : other.GetNextVertices(from)) {
                adjacencyMatrix[from][to] = true;
            }
        }
    }

    void AddEdge(int from, int to) override { // Добавляем вершину
        if (!isValidIndex(from) || !isValidIndex(to)) {
            throw std::out_of_range("Invalid index");
        }
        adjacencyMatrix[from][to] = true;
    }

    int VerticesCount() const override { // Получаем размер
        return adjacencyMatrix.size(); 
    }

    std::vector<int> GetNextVertices(int vertex) const override { // Получаем смежные вершины
        if (!isValidIndex(vertex)) {
            throw std::out_of_range("Invalid index");
        }

        std::vector<int> nextVertices; 
        for (int to = 0; to < VerticesCount(); to++) { // Все возможные "пункты назначения"
            if (adjacencyMatrix[vertex][to]) { // Если элемент матрицы true
                nextVertices.push_back(to); // Добавляем в вектор
            }
        }
        return nextVertices;
    }

    std::vector<int> GetPrevVertices(int vertex) const override { // Получение всех вершин, из которых есть ребро в заданную
        if (!isValidIndex(vertex)) {
            throw std::out_of_range("Invalid index");
        }

        std::vector<int> prevVertices; 
        for (int from = 0; from < VerticesCount(); from++) { // Такая же лошика, как в GetNextVertices, только наоборот
            if (adjacencyMatrix[from][vertex]) {
                prevVertices.push_back(from);
            }
        }
        return prevVertices;
    }
};

void runMatrixTests(std::istream& input, std::ostream& output) {
    // Тест 1: Создание графа с заданным количеством вершин
    {
        MatrixGraph graph(5);
        if (graph.VerticesCount() != 5) {
            output << "Test 1 FAILED: Wrong vertices count\n";
        }
        else {
            output << "Test 1 PASSED: Correct vertices count\n";
        }
    }

    // Тест 2: Добавление ребер и проверка их наличия
    {
        MatrixGraph graph(3);
        graph.AddEdge(0, 1);
        graph.AddEdge(1, 2);

        auto next0 = graph.GetNextVertices(0);
        if (next0 != std::vector<int>{1}) {
            output << "Test 2 FAILED: Wrong next vertices for vertex 0\n";
        }
        else {
            output << "Test 2 PASSED: Correct edge addition\n";
        }
    }

    // Тест 3: Проверка предыдущих вершин (важный для матрицы!)
    {
        MatrixGraph graph(4);
        graph.AddEdge(0, 3);
        graph.AddEdge(1, 3);
        graph.AddEdge(2, 3);

        auto prev3 = graph.GetPrevVertices(3);
        std::sort(prev3.begin(), prev3.end());
        if (prev3 != std::vector<int>{0, 1, 2}) {
            output << "Test 3 FAILED: Wrong previous vertices\n";
        }
        else {
            output << "Test 3 PASSED: Correct previous vertices\n";
        }
    }

    // Тест 4: Проверка на петли (ребро из вершины в себя)
    {
        MatrixGraph graph(2);
        graph.AddEdge(0, 0);

        auto next = graph.GetNextVertices(0);
        if (next != std::vector<int>{0}) {
            output << "Test 4 FAILED: Loop edge not handled\n";
        }
        else {
            output << "Test 4 PASSED: Loop edge handled\n";
        }
    }

    // Тест 5: Попытка добавить ребро с неверным индексом
    {
        MatrixGraph graph(2);
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

    // Тест 6: Копирование графа (важный для матрицы!)
    {
        MatrixGraph graph1(3);
        graph1.AddEdge(0, 1);
        graph1.AddEdge(1, 2);

        MatrixGraph graph2(graph1);

        if (graph2.GetNextVertices(0) != std::vector<int>{1}) {
            output << "Test 6 FAILED: Copy constructor problem\n";
        }
        else {
            output << "Test 6 PASSED: Graph copied correctly\n";
        }
    }

    // Тест 7: Проверка на изолированную вершину
    {
        MatrixGraph graph(3);
        graph.AddEdge(0, 1);
        graph.AddEdge(1, 2);

        auto next2 = graph.GetNextVertices(2);
        if (!next2.empty()) {
            output << "Test 7 FAILED: Isolated vertex problem\n";
        }
        else {
            output << "Test 7 PASSED: Isolated vertex handled\n";
        }
    }

    // Тест 8: Плотный граф (важный для матрицы!)
    {
        const int SIZE = 10;
        MatrixGraph graph(SIZE);

        // Создаем полный граф
        for (int i = 0; i < SIZE; ++i) {
            for (int j = 0; j < SIZE; ++j) {
                if (i != j) graph.AddEdge(i, j);
            }
        }

        bool correct = true;
        for (int i = 0; i < SIZE; ++i) {
            if (graph.GetNextVertices(i).size() != SIZE - 1) {
                correct = false;
                break;
            }
        }

        if (!correct) {
            output << "Test 8 FAILED: Dense graph problem\n";
        }
        else {
            output << "Test 8 PASSED: Dense graph handled\n";
        }
    }

    // Тест 9: Проверка на отсутствие ложных ребер
    {
        MatrixGraph graph(3);
        graph.AddEdge(0, 1);

        bool hasFalseEdge = false;
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                if (i == 0 && j == 1) continue;
                if (graph.GetNextVertices(i).size() != (i == 0 ? 1 : 0)) {
                    hasFalseEdge = true;
                }
            }
        }

        if (hasFalseEdge) {
            output << "Test 9 FAILED: False edges detected\n";
        }
        else {
            output << "Test 9 PASSED: No false edges\n";
        }
    }

    // Тест 10: Производительность с большим графом
    {
        const int SIZE = 100;
        MatrixGraph graph(SIZE);

        // Добавляем ребра в шахматном порядке
        for (int i = 0; i < SIZE; ++i) {
            for (int j = 0; j < SIZE; ++j) {
                if ((i + j) % 2 == 0) {
                    graph.AddEdge(i, j);
                }
            }
        }

        int edgeCount = 0;
        for (int i = 0; i < SIZE; ++i) {
            edgeCount += graph.GetNextVertices(i).size();
        }

        if (edgeCount != SIZE * SIZE / 2 + (SIZE % 2)) {
            output << "Test 10 FAILED: Large graph problem\n";
        }
        else {
            output << "Test 10 PASSED: Large graph handled\n";
        }
    }
}

int TestMatrixGraph() {
    runMatrixTests(std::cin, std::cout);
    return 0;
}
