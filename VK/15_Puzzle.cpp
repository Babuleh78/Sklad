#include <iostream>
#include <set>
#include <map>
#include <string>
#include <vector>

//Написать алгоритм для решения игры в “пятнашки”.Решением задачи является приведение к виду :
//[1 2 3 4] [5 6 7 8] [9 10 11 12] [13 14 15 0] , где 0 задает 
//пустую ячейку.Достаточно найти хотя бы какое - то решение.
//Число перемещений костяшек не обязано быть минимальным.


class Board { // Класс доски, где поле пятнашек 4x4 представлено как массив 1x16
public:

    Board(std::vector<int>& g) { // Конструктор из одномерного вектора
        
        for (auto i : g) { // Заполняем начальную доску
            StartBoard.push_back(i);
        }
        FinalBoard = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 0 }; // Решенная доска
    }

    bool isSolutionExists() { // Функция проверки существования решения
        int inversionCount = 0;
        for (int i = 0; i < 16; i++) { // Считаем количество инверсий (попарно)
            if (StartBoard[i]) { // Если не пустая клетка
                for (int j = 0; j < i; j++) {
                    if (StartBoard[j] > StartBoard[i]) {
                        inversionCount += 1;
                    }
                }
            }
        }
        for (int i = 0; i < 16; i++) {
            if (StartBoard[i] == 0) { // Обработка пустого мета
                inversionCount += 1 + i / 4;
            }
        }
        return (inversionCount % 2) == 0; // Если число инверсий четно - разрешимо
    }

    char WhichMove(std::vector<int> from, std::vector<int> to) { // Определяем какое движение было совершено 

        int FirstSpacePosition = 0; // Позиция нуля в первом 
        int SecSpacePosition = 0; // И во втором случае

        for (int i = 0; i < 16; i++) {  // Находим ноль в первом случае
            if (from[i] == 0) {
                FirstSpacePosition = i;
                break;
            }
        }
        for (int i = 0; i < 16; i++) { // И во втором
            if (to[i] == 0) {
                SecSpacePosition = i;
                break;
            }
        }

        switch (SecSpacePosition - FirstSpacePosition) { // В зависимости от значения определяем, какое было передвижение
            case 1: return 'R';
            case -1: return 'L';
            case 4: return 'D';
            case -4: return 'U';
            default: return 0;
        }
    }

    void GetNextVert(std::vector<int>& v, std::vector<std::vector<int>>& next) { // Генерация всех возможных состояний доски 
        int SpacePosition = 0;
        for (int i = 0; i < 16; i++) { // Находим позицию пустой клетки
            if (v[i] == 0) {
                SpacePosition = i;
                break;
            }
        }
        int i = 0;
        if (SpacePosition < 15 && SpacePosition % 4 != 3) { // Двигаем вправо, если пустая клетка не в последнем столбце
            next.push_back(v);
            std::swap(next[i][SpacePosition], next[i][SpacePosition + 1]);
            i++;
        }

        if (SpacePosition > 0 && SpacePosition % 4 != 0) { // Двигаем влево, если пустая клетка не в первом столбце
            next.push_back(v);
            std::swap(next[i][SpacePosition], next[i][SpacePosition - 1]);
            i++;
        }

        if (SpacePosition >= 4) { // Двигаем вверх, если пусткая клетка не в первой строке
            next.push_back(v);
            std::swap(next[i][SpacePosition], next[i][SpacePosition - 4]);
            i++;
        }

        if (SpacePosition <= 11) { // Двигаем вниз, если пустая клетка не в последней строке
            next.push_back(v);
            std::swap(next[i][SpacePosition], next[i][SpacePosition + 4]);
        }
    }

    int CountTheHeuristic(std::vector<int> v) { // Считаем Манхэттонское расстояние для текущего состояния (минимальное количество ходов для решения)
        int Heuristic = 0;
        for (int i = 0; i < v.size(); i++) {
            int elem = v[i];
            if (elem == 0) { // Если клетка пустая
                Heuristic += abs(3 - i / 4) + abs(3 - i % 4); // Считаем расстояние до правого нижнего угла
            }
            else { // Если на клетке пятнашка
                Heuristic += abs((elem - 1) / 4 - i / 4) + abs((elem - 1) % 4 - i % 4); // Считаем расстояние между текущим и целевым расстоянием
            }
        }
        return Heuristic;
    }

    const std::vector<int>& getStart() const {
        return StartBoard;
    }
    const std::vector<int>& getFinal() const {
        return FinalBoard;
    }

private:
    std::vector<int> StartBoard; // Доска, которую необходимо решить
    std::vector<int> FinalBoard; // Решенное состояние
};


std::string SolvePuzzle(Board& board) { // Функция для решения задачи

   
    if (!board.isSolutionExists()) { // Если решения изначально не существует
        return "-1";
    }

    std::set<std::pair<int, std::vector<int>>> q; // Очередь для обрабокти состояний (элементы отсортированы по возрастанию)
    std::set<std::vector<int>> checked; // Посещенные доски
    std::map<std::vector<int>, int> cost; // Стоимость пути от начала
    std::map<std::vector<int>, int> f; // Функция оценки f = cost+h (h - минимальное количество ходов)
    std::map<std::vector<int>, std::vector<int>> parents; // Родительские состояния

    std::vector<int> Start = board.getStart(); // Начальное состояние доски
    std::vector<int> Final = board.getFinal(); // Целевое состояние

    cost[Start] = 0; // Стоимость пути до начального состояния
    f[Start] = cost[Start] + board.CountTheHeuristic(Start); // Суммируем стоимость и мин количество ходов
    q.insert({ f[Start], Start }); // Добавляем пару стоимость - положение

    while (!q.empty()) { // Пока не обработали все состояние
        std::vector<int> currentBoard = q.begin()->second; // Извлекаем состояние с наименьшей функцией f
        q.erase(q.begin()); // Удаляем его из множества

        if (currentBoard == Final) { // Если достигли конечного состояния

            std::string answer = ""; // Формируем ответ
            int NumOfMoves = 0; // Который состоит из числа ходов
            std::string ReverseWay; // И маршрута

            std::vector<int> g = std::vector<int>(16, 0);

            Board tempBoard(g);

            while (currentBoard != Start) { // Пока не вернемся в начальное состояние
                NumOfMoves++; // Увеличиваем число ходов
                std::vector<int> prev = parents.at(currentBoard); // Получаем предыдущее состояние
                ReverseWay += tempBoard.WhichMove(currentBoard, prev); // Получаем перемещение
                currentBoard = prev; // Откатываемся на ход назад
            }

            answer += std::to_string(NumOfMoves) + "\n"; // Добавляем число перемещений в ответ
            for (int i = (int)ReverseWay.length() - 1; i >= 0; i--) { // И Добавляем в ответ последовательность
                answer+= ReverseWay[i];
            }
            return answer;
            
        }

        checked.insert(currentBoard); // Посещаем состояние
        std::vector<std::vector<int>> NextVert; // Все возможные следующих состояние
        board.GetNextVert(currentBoard, NextVert); // Генерация всех следующих состояний

        for (auto v : NextVert) { // Для каждого варианта
            int TentScore = cost[currentBoard] + 1; // Стоимость увеличивается на 1 
            if (checked.find(v) != checked.end() && TentScore >= cost[v]) { // Если мы здесь уже были или маршрут не лучший, продолжаем обработку
                continue;
            }
            if (checked.find(v) == checked.end() || TentScore < cost[v]) { // Если состояние не обработано или более выгодно
                parents[v] = currentBoard; // Добавляем текущее состояние в родительские состояния
                cost[v] = TentScore; // Обновляем стоимость
                f[v] = cost[v] + board.CountTheHeuristic(v); // Обновляем функцию оценки
                if (q.find({ f[v] - cost[v], v }) == q.end()) { // Если состояние новое
                    q.insert({ f[v] - cost[v], v }); // Добавляем его в очередь с соответствущим значением
                }
            }
        }
    }
}

void run(std::istream& input, std::ostream& output) {

    std::vector<int> tmp; 
    for (int i = 0; i < 16; i++) { // Заполняем поле
        int value = 0;
        std::cin >>value;
        tmp.push_back(value);
    }
    Board board(tmp);
    output << SolvePuzzle(board); // Передаем в функцию решения
}


int main() {

    run(std::cin, std::cout);
    return 0;
   
}
