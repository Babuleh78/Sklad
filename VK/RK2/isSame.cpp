#include <iostream>
#include <string>
#include <stack>
#include <queue>

//Напишите функцию, проверяющую, что во 
//всех узлах бинарного дерева поиска хранится одинаковое значение.
//Значения - int.




template<typename T, typename Cmp>
class BinaryTree {
private:

    struct Node { // Лист бинарного дерева
        Node* left; // Указатель на левый лист
        Node* right; // Указатель на правый лист
        T value; // Значение листа
        Node(const T& _value) : left(nullptr), right(nullptr), value(_value) {} // Конструктор по умолчанию
    };

    Node* root; // Корень дерева
    Cmp cmp; // Компаратор

    void deleteNode(Node* node) { // Удаление листа и всех его потомков
        if (node == nullptr) {
            return;
        }

        std::stack<Node*> stack; // Задаем стек для удаления всех листов
        stack.push(node); // Добавляем исходный лист

        while (!stack.empty()) { // Пока не пройдемся по всем потомкам исходного node
            Node* current_node = stack.top(); // Получаем лист с вершины стэка
            stack.pop(); // Удаляем верхний элемент стэка

            if (current_node->left) { // Если у листа есть потомок слева
                stack.push(current_node->left); // Добавляем его в стек
            }

            if (current_node->right) { // Если у листа есть потомок справа
                stack.push(current_node->right); // Добавляем его в стек
            }

            delete current_node; // Очищаем память
        }
    }

    void add(Node*& root, const T& _value) { // Функция добавления в дерево
        if (!root) { // Если еще нет ни одного листа
            root = new Node(_value); // Создаем новый лист
            return;
        }

        Node* current_node = root;
        while (current_node) { // Проходимся по всем элементам дерева
            if (cmp(_value, current_node->value)) { // Если вставляемый элемент больше значения текущего листа
                if (!current_node->left) { // Если лист не существует
                    current_node->left = new Node(_value); // Создаем новый лист
                    return; // И выходим из цикла
                }
                current_node = current_node->left; // Если существует, то идем дальше
            }
            else {
                if (!current_node->right) { // Если лист не существует
                    current_node->right = new Node(_value); // Создаем новый лист
                    return; // И выходим из цикла
                }
                current_node = current_node->right; // Если существует, то идем дальше
            }
        }
    }

    bool has(Node* root, const T& _value) const { // Функция проверки наличия значения в дереве
        if (!root) { // Если дерево пустое
            return false;
        }

        Node* current_node = root;
        while (current_node) { // Проходимся по всем элементам дерева
            if (current_node->value == _value) { // Если нашли значение
                return true; // Возвращаем true
            }

            if (cmp(_value, current_node->value)) { // Если значение value меньше значение листа
                current_node = current_node->left; // Идем влево
            }
            else { // Иначе
                current_node = current_node->right; // Идем вправо
            }
        }

        return false; // Если прошли по всем элементам и не нашли _value возвращаем false
    }

    void traverse(Node* root, void (*visit) (const T&)) const {
        if (!root) return; // Если дерево пустое выходим

        std::stack<Node*> s; // Создаем стэк
        Node* last_visited = nullptr;  // Последний посещённый узел

        Node* current_node = root;
        while (current_node || !s.empty()) { // Пока существует лист или стэк не пустой
            if (current_node) { // Если лист существует
                s.push(current_node); // Добавляем его в стек
                current_node = current_node->left;  // И идём до самого левого узла
            }
            else { // Иначе
                Node* top_node = s.top(); // Получаем верхний лист

                if (top_node->right && top_node->right != last_visited) {  // Если правый потомок существует и ещё не посещён
                    current_node = top_node->right;  // Переходим в правое поддерево
                }
                else { // Иначе
                    visit(top_node->value);     // Посещаем узел 
                    last_visited = top_node;    // Запоминаем, что посетили данный узел
                    s.pop();                   // Убираем узел из стека
                }
            }
        }
    }

    bool AllValuesSame() const {
        if (root == nullptr) { // Если значений нет
            return true; 
        }

        T first_value = root->value; // Исходное значение
        std::stack<Node*> stack; // Очередь для узлов
        Node* current = root;

        while (current != nullptr || !stack.empty()) { // Пока не пройдемся по всему дереву и не рассмторим все узлы дерева
            while (current != nullptr) { // Пока узел есть
                stack.push(current); // Заносим в очередь
                current = current->left; // Начинаем проход по левым узлам
            }

            current = stack.top(); // Получаем верхний узел
            stack.pop();

            if (current->value != first_value) { // Если он не равен исходному значению
                return false; // Значит числа разные
            }

            current = current->right; // Проходим по остальным узлам, т.е. правым
        }

        return true;
    }

public:
    BinaryTree(const Cmp& _cmp) : root(nullptr), cmp(_cmp) {} // Конструктор по умолчанию
    ~BinaryTree() { // Деструктор
        deleteNode(root);
    }

    BinaryTree(const BinaryTree&) = delete; // Запрещаем копирование
    BinaryTree& operator=(const BinaryTree&) = delete; // И присваивание

    void Add(const T& _value) { // Публичный метод добавления
        add(root, _value);
    }

    bool Has(const T& _value) { // Публичный метод проверки наличия элемента
        return has(root, _value);
    }

    void Traverse(void (*visit) (const T&)) const {
        traverse(root, visit);
    }

    bool CheckValue() {
        return AllValuesSame();
    }
};

struct IntCmp { // Компаратор для int
    bool operator()(int l, int r) const { return l < r; }
};



void run(std::istream& input, std::ostream& output) {

    IntCmp cmp;
    BinaryTree<int, IntCmp> tree(cmp);

    int value;
    while (std::cin >> value) {
        tree.Add(value);
    }

    if (tree.CheckValue()) {
        output << "1";
    }
    else {
        output << "0";
    }

}

int main() {
    run(std::cin, std::cout);
    return 0;
}
