#include <iostream>
//Дано число N и N строк. Каждая строка содержащит команду
//добавления или удаления натуральных чисел, а также запрос
//на получение k-ой порядковой статистики. Команда добавления 
//числа A задается положительным числом A, команда удаления
//числа A задается отрицательным числом “-A”. Запрос
//на получение k-ой порядковой статистики задается числом k.
//Требуемая скорость выполнения запроса - O(log n).

template<typename T, typename Cmp>
class AVLTree {

private:

    struct Node { // Узел АВЛ дерева
        Node* left; // Указатель на левый дочерний элемент
        Node* right; // Указатель на правый дочерний элемент
        T value; // Значение листа
        size_t height; // "Высота" данного узла
        size_t size;  // Количество узлов в поддереве

        ~Node() {
            delete left;   // Рекурсивное удаление левого поддерева
            delete right;  // Рекурсивное удаление правого поддерева
        }
        Node( const T& _value) : value(_value), left(nullptr), right(nullptr), height(1), size(1) {}
    };

    Cmp cmp; // Компаратор
    Node* root; // Корень дерева

    size_t get_height(const Node* node) const { // Вспомогательный способ получения высоты, чтобы не пытаться получить высоту у nullptr
        return node == nullptr ? 0 : node->height;
    }

    size_t get_size(const Node* node) const { // Вспомогательный способ получения размера, чтобы не пытаться получить размер у nullptr
        return node == nullptr ? 0 : node->size;
    }

    int BalanceFactor(const Node* node) const { // Функция для подсчета необходимости балансировки
        if (!node) {
            return 0;
        }
        int diff = get_height(node->right) - get_height(node->left); // Возвращает разницу высот правого и левого поддеревьев

        return diff;
    }

    void FixHeight(Node* node) {  // Обновление высоты для узла
        size_t h_left = get_height(node->left);
        size_t h_right = get_height(node->right);
        node->height = std::max(h_left, h_right) + 1; // Обновление высоты узла на основе высот его поддеревьев
    }

    void FixSize(Node* node) { // Обновление размера для узла
        size_t l_size = get_size(node->left);
        size_t r_size = get_size(node->right);
        node->size = l_size + r_size + 1; // Обновление размера поддерева узла
    }

    void FixSizeAndHeight(Node* node) { // Объединение Fix-ов
        FixHeight(node);
        FixSize(node);
    }

    Node* rotateRight(Node* node) { // Функция правого поворота
        Node* left = node->left; // Сохраняем левого потомка
        node->left = left->right; // Перемещаем правое поддерево левого потомка
        left->right = node; // Делаем исходный узел правым потомком

        FixSizeAndHeight(node);

        FixSizeAndHeight(left);

        return left; // Возвращаем новый корень
    }

    Node* rotateLeft(Node* node) { // Функция левого поворота
        Node* right = node->right; // Сохраняем правого потомка
        node->right = right->left; // Перемещаем левое поддерево правого потомка
        right->left = node; // Делаем исходный узел левым потомком

        FixSizeAndHeight(node);

        FixSizeAndHeight(right);

        return right;  // Возвращаем новый корень
    }

    Node* balance(Node* node) { // Функция балансировки

        FixSizeAndHeight(node); // Балансируем размер и высоту

        if (BalanceFactor(node) == 2) { // Требуется балансировка левым поворотом
            if (BalanceFactor(node->right) < 0) { // Обработка двойного дисбаланса
                node->right = rotateRight(node->right);
            }
            return rotateLeft(node);
        }
        if (BalanceFactor(node) == -2) { // Требуется балансировка правым поворотом
            if (BalanceFactor(node->left) > 0) { // Обработка двойного дисбаланса
                node->left = rotateLeft(node->left);
            }
            return rotateRight(node);
        }
        return node;
    }

    Node* insert(Node* root, const T& value) { // Вставка в дерево
        if (!root) { // Если дерево пустое
            return new Node(value);
        }
        else if (cmp(value, root->value)) {
            root->left = insert(root->left, value);
        }
        else {
            root->right = insert(root->right, value);
        }

        return balance(root); // Балансируем дерево
    }

    Node* findMin(Node* node) { // Ищем самый маленький узел
        if (!node) {
            return nullptr;
        }

        while (node->left != nullptr) {
            node = node->left;
        }

        return node;
    }

    Node* removeMin(Node* node) { // Удаление минимального значения
        if (!node) {
            return nullptr;
        }

        Node* parent_node = nullptr;
        Node* current_node = findMin(node);


        if (!parent_node) {  // Если минимальный узел - это корень поддерева
            Node* rightChild = current_node->right;
            delete current_node;
            return rightChild;
        }

        parent_node->left = current_node->right; // Удаляем минимальный узел, поднимая его правое поддерево
        delete current_node; // Очищаем память


        return balance(node); // Балансируем дерево
    }

    Node* findAndRemoveMin(Node* node, Node*& minNode) {
        if (!node->left) {
            minNode = node;
            return node->right;
        }
        node->left = findAndRemoveMin(node->left, minNode);
        return balance(node);
    }

    Node* remove(Node* node, const T& value) {
        if (!node) { // Если значение не найдено
            return nullptr;
        }

        if (cmp(value, node->value)) {  // Поиск узла для удаления в левом поддереве
            node->left = remove(node->left, value);
        }
        else if (!cmp(value, node->value) && value != node->value) { // Поиск узла для удаления в правом поддереве
            node->right = remove(node->right, value);
        }
        else {
            if (node->left == nullptr || node->right == nullptr) { // У узла нет одного или обоих потомков
                Node* temp = node->left ? node->left : node->right; // Берем существующего потомка

                if (!temp) { // Нет потомков
                    temp = node;
                    node = nullptr;
                }
                else { // Один потомок
                    *node = *temp; // Копируем данные потомка

                    temp->left = temp->right = nullptr; // Обнуляем указатели перед удалением
                    delete temp;
                }
            }
            else { // Eсть оба потомка
                Node* minNode;
                node->right = findAndRemoveMin(node->right, minNode);
                node->value = minNode->value;

                minNode->left = minNode->right = nullptr;
                delete minNode;
            }
        }

        if (!node) {
            return node;
        }

        return balance(node); // Балансируем дерево
    }
public:


    AVLTree(Cmp cmp) : cmp(cmp), root(nullptr) {} // Конструктор по умолчанию
    ~AVLTree() { delete root; } // Деструктор

    void Insert(const T& value) { // Метод вставки
        root = insert(root, value);
    }

    void Remove(const T& value) {
        root = remove(root, value);
    }

    const T& kthOrderStatistic(size_t k) const { // Получаем k-ю статистику
        const T* result = nullptr;

        Node* node = root;
        size_t index = get_size(root->left); // Получаем размер элемента

        while (index != k) {
            if (k > index) { // Сдвигаемся вправо
                node = node->right;
                index += 1 + get_size(node->left);
            }
            else { // Иначе сдвигаемся влево
                node = node->left;
                index -= 1 + get_size(node->right);
            }
        }
        result = &node->value;


        return *result;
    }

};

struct IntCmp { // Компаратор для int
    bool operator()(int l, int r) const { return l < r; }
};



void run(std::istream& input, std::ostream& output) {
    IntCmp cmp;

    AVLTree<int, IntCmp> Tree(cmp);
    int n;
    input >> n;

    int command = 0;
    int k_request = 0;

    for (int i = 0; i < n; i++) {
        int command, k_request;
        input >> command >> k_request;

        if (command > 0) {
            Tree.Insert(command);
        }
        else {
            Tree.Remove(-command);
        }

        output << Tree.kthOrderStatistic(k_request) << " ";
    }


}



int main() {
    run(std::cin, std::cout);
    return 0;
}
