#include <iostream>
#include <iostream>
#include <unordered_set>
#include <string>


//Нужно кастомизировать std::unordered_set для операций со структурой данных
//
//struct Node {
//    std::string product_name;
//    int color;
//    int size;
//};
//
//Для этого нужно сделать :
//-специализацию std::hash для Node
//- определить оператор ==
//
//Как сделать специализацию std::hash :
//
//    namespace std {
//    template<>
//    struct hash<Node> {
//        std::size_t operator()(const Node& node) const noexcept {
//            // ...
//        }
//    };
//}


struct Node {
    std::string product_name;
    int color;
    int size;

    bool operator==(const Node& other) const { // Определение оператора
        return product_name == other.product_name && color == other.color && size == other.size;
    }
};

namespace std { // Специадизация std::hash
    template<>
    struct hash<Node> {
        std::size_t operator()(const Node& node) const noexcept { 
            std::size_t h1 = std::hash<std::string>{}(node.product_name); // Вычисляем хэш для названия
            std::size_t h2 = std::hash<int>{}(node.color); // Для цвета
            std::size_t h3 = std::hash<int>{}(node.size); // И для размера
            return h1 * 31 + h2 * 17 + 23 * h3; // Используем простые числа для перемешивания
        }
    };
}



void run(std::istream& input, std::ostream& output) {

    std::unordered_set<Node> set;
    char operation;

    std::string product_name;
    int color, size;

    while ( std::cin >> operation >> product_name >> color >> size ) { // Пока поступают данные
        Node node{ product_name, color, size };

        switch (operation) { // Обрабатываем каждую из операция 
        case '+':
            set.insert(node);
            output << "OK" << std::endl;
            break;
        case '-':
            if (set.erase(node)) {
                output << "OK" << std::endl;
            }
            else {
                output << "FAIL" << std::endl;
            }
            break;
        case '?':
            if (set.find(node) != set.end()) {
                output << "OK" << std::endl;
            }
            else {
                output << "FAIL" << std::endl;
            }
            break;
        default:
            output << "FAIL" << std::endl;
            break;
        }
    }


}

int main() {
    run(std::cin, std::cout);
    return 0;
}
