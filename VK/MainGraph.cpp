#include "IGraph.h"
#include "ListGraph.h"
#include "MatrixGraph.h"
#include "SetGraph.h"
#include "ArcGraph.h"

#include <iostream>
#include <cassert>



void TestListGraph(std::istream& input, std::ostream& output) {
    // Тест 1: Создание графа и проверка количества вершин
    {
        ListGraph graph(5);
        if (graph.VerticesCount() != 5) {
            output << "Test 1 FAILED: Wrong vertices count\n";
        }
        else {
            output << "Test 1 PASSED: Vertices count correct\n";
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
            output << "Test 2 FAILED: Wrong next vertices for vertex 0\n";
        }
        else {
            output << "Test 2 PASSED: Next vertices correct\n";
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
            output << "Test 3 FAILED: Wrong previous vertices for vertex 1\n";
        }
        else {
            output << "Test 3 PASSED: Previous vertices correct\n";
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
            output << "Test 4 FAILED: No exception for invalid index\n";
        }
        else {
            output << "Test 4 PASSED: Exception thrown for invalid index\n";
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
            output << "Test 5 FAILED: Wrong vertices count after copy\n";
        }
        else {
            auto next1 = graph2.GetNextVertices(1);
            if (next1 != std::vector<int>{2}) {
                output << "Test 5 FAILED: Wrong edges after copy\n";
            }
            else {
                output << "Test 5 PASSED: Graph copied correctly\n";
            }
        }
    }

    // Тест 6: Пустой граф
    {
        ListGraph graph(0);
        if (graph.VerticesCount() != 0) {
            output << "Test 6 FAILED: Empty graph has non-zero size\n";
        }
        else {
            output << "Test 6 PASSED: Empty graph handled correctly\n";
        }
    }

    // Тест 7: Добавление петли
    {
        ListGraph graph(3);
        graph.AddEdge(1, 1); // Петля
        auto next = graph.GetNextVertices(1);
        if (next != std::vector<int>{1}) {
            output << "Test 7 FAILED: Loop edge not handled correctly\n";
        }
        else {
            output << "Test 7 PASSED: Loop edge handled correctly\n";
        }
    }

    // Тест 8: Добавление кратных ребер
    {
        ListGraph graph(3);
        graph.AddEdge(0, 1);
        graph.AddEdge(0, 1); // Дублирование ребра
        auto next = graph.GetNextVertices(0);
        if (next.size() != 2 || next[0] != 1 || next[1] != 1) {
            output << "Test 8 FAILED: Multiple edges not handled correctly\n";
        }
        else {
            output << "Test 8 PASSED: Multiple edges handled correctly\n";
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
            output << "Test 9 FAILED: Large graph handling problem\n";
        }
        else {
            output << "Test 9 PASSED: Large graph handled correctly\n";
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
            output << "Test 10 FAILED: Assignment operator vertices count\n";
        }
        else {
            auto next = graph2.GetNextVertices(0);
            if (next != std::vector<int>{1}) {
                output << "Test 10 FAILED: Assignment operator edges\n";
            }
            else {
                output << "Test 10 PASSED: Assignment operator works correctly\n";
            }
        }
    }

}

void TestMatrixGraph(std::istream& input, std::ostream& output) {
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


void TestSetGraph(std::istream& input, std::ostream& output) {
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

void TestArcGraph(std::istream& input, std::ostream& output) {
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


void TestListGraphCopyConstructor() {
    // 1. Копирование из MatrixGraph
    MatrixGraph matrix(5);
    matrix.AddEdge(0, 1);
    matrix.AddEdge(1, 2);
    matrix.AddEdge(2, 3);

    ListGraph listFromMatrix(matrix);
    assert(listFromMatrix.VerticesCount() == 5);
    assert(listFromMatrix.GetNextVertices(0) == std::vector<int>{1});
    assert(listFromMatrix.GetPrevVertices(1) == std::vector<int>{0});

    // 2. Копирование из SetGraph
    SetGraph set(4);
    set.AddEdge(0, 1);
    set.AddEdge(1, 2);
    set.AddEdge(2, 3);

    ListGraph listFromSet(set);
    assert(listFromSet.VerticesCount() == 4);
    assert(listFromSet.GetNextVertices(1) == std::vector<int>{2});

    // 3. Копирование из ArcGraph
    ArcGraph arc(3);
    arc.AddEdge(0, 1);
    arc.AddEdge(1, 2);

    ListGraph listFromArc(arc);
    assert(listFromArc.VerticesCount() == 3);
    assert(listFromArc.GetPrevVertices(2) == std::vector<int>{1});
}

void TestMatrixGraphCopyConstructor() {
    // 1. Копирование из ListGraph
    ListGraph list(4);
    list.AddEdge(0, 1);
    list.AddEdge(1, 2);
    list.AddEdge(2, 3);

    MatrixGraph matrixFromList(list);
    assert(matrixFromList.VerticesCount() == 4);
    assert(matrixFromList.GetNextVertices(1) == std::vector<int>{2});

    // 2. Копирование из SetGraph
    SetGraph set(3);
    set.AddEdge(0, 1);
    set.AddEdge(1, 2);

    MatrixGraph matrixFromSet(set);
    assert(matrixFromSet.VerticesCount() == 3);
    assert(matrixFromSet.GetPrevVertices(2) == std::vector<int>{1});

    // 3. Копирование из ArcGraph
    ArcGraph arc(2);
    arc.AddEdge(0, 1);

    MatrixGraph matrixFromArc(arc);
    assert(matrixFromArc.VerticesCount() == 2);
    assert(matrixFromArc.GetNextVertices(0) == std::vector<int>{1});
}

void TestSetGraphCopyConstructor() {
    // 1. Копирование из ListGraph
    ListGraph list(3);
    list.AddEdge(0, 1);
    list.AddEdge(1, 2);

    SetGraph setFromList(list);
    assert(setFromList.VerticesCount() == 3);
    assert(setFromList.GetNextVertices(1) == std::vector<int>{2});

    // 2. Копирование из MatrixGraph
    MatrixGraph matrix(4);
    matrix.AddEdge(0, 1);
    matrix.AddEdge(1, 2);
    matrix.AddEdge(2, 3);

    SetGraph setFromMatrix(matrix);
    assert(setFromMatrix.VerticesCount() == 4);
    assert(setFromMatrix.GetPrevVertices(3) == std::vector<int>{2});

    // 3. Копирование из ArcGraph
    ArcGraph arc(2);
    arc.AddEdge(0, 1);

    SetGraph setFromArc(arc);
    assert(setFromArc.VerticesCount() == 2);
    assert(setFromArc.GetNextVertices(0) == std::vector<int>{1});
}

void TestArcGraphCopyConstructor() {
    // 1. Копирование из ListGraph
    ListGraph list(3);
    list.AddEdge(0, 1);
    list.AddEdge(1, 2);

    ArcGraph arcFromList(list);
    assert(arcFromList.VerticesCount() == 3);
    assert(arcFromList.GetNextVertices(1) == std::vector<int>{2});

    // 2. Копирование из MatrixGraph
    MatrixGraph matrix(2);
    matrix.AddEdge(0, 1);

    ArcGraph arcFromMatrix(matrix);
    assert(arcFromMatrix.VerticesCount() == 2);
    assert(arcFromMatrix.GetPrevVertices(1) == std::vector<int>{0});

    // 3. Копирование из SetGraph
    SetGraph set(4);
    set.AddEdge(0, 1);
    set.AddEdge(1, 2);
    set.AddEdge(2, 3);

    ArcGraph arcFromSet(set);
    assert(arcFromSet.VerticesCount() == 4);
    assert(arcFromSet.GetNextVertices(2) == std::vector<int>{3});
}

int main() {
    TestListGraph(std::cin, std::cout);
    TestMatrixGraph(std::cin, std::cout);
    TestSetGraph(std::cin, std::cout);
    TestArcGraph(std::cin, std::cout);

    TestListGraphCopyConstructor();
    TestMatrixGraphCopyConstructor();
    TestSetGraphCopyConstructor();
    TestArcGraphCopyConstructor();

    std::cout << "All copy constructor tests passed!" << std::endl;

    return 0;
}
