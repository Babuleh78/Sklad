#include <iostream>
#include <vector>
#include <stack>
#include <queue>
//Постройте B - дерево минимального порядка t и выведите его по слоям.
//В качестве ключа используются числа, лежащие в диапазоне 0..232 - 1
//
//Требования:
//B - дерево должно быть реализовано в виде шаблонного класса.
//Решение должно поддерживать передачу функции сравнения снаружи.


template<typename T, typename Cmp>
class BTree { 
private:
    struct Node { // Элемент B дерева
        bool leaf; // Является ли узел листов (не имеет дочерних элементов)
        std::vector<T> values; // Вектор значений
        std::vector<Node*> children; // Вектор указателей на дочерние узлы

        Node( bool is_leaf ) : leaf( is_leaf ) {} // Конструктор по умолчанию

        ~Node() {
            for ( Node * child : children ) { // Очищение памяти
                delete child;
            }
        }
    };

    int t; // Минимальный порядок дерева
    Node * root; // Корень дерева
    Cmp cmp; // Компаратор

    void split_child( Node * parent, int index ) { // разделение переполненного дочернего узла
        Node * child = parent->children[ index ]; // Переполненный ребенок
        Node * new_child = new Node( child->leaf ); // Новый узел для разделения

        new_child->values.assign( child->values.begin() + t, child->values.end() ); // Переносим t-1 ключей и t детей из старого узла в новый
        if ( !child->leaf ) { // Если у переполненного ребенка были дочерние элементы
            new_child->children.assign( child->children.begin() + t, child->children.end() ); // Добавляем их
        }

        parent->values.insert( parent->values.begin() + index, child->values[t - 1] ); // Вставляем средний ключ в родителя

       
        child->values.resize( t - 1 );  // Обновляем вектор значений
        if ( !child->leaf ) { // Обновляем вектор дочерних элементов
            child->children.resize( t );
        }

       
        parent->children.insert( parent->children.begin() + index + 1, new_child);  // Вставляем нового ребенка в родителя
    }

    void add_non_full( Node* root, const T& value ) { // Вставка в незаполненный узел
        std::stack<std::pair<Node*, int>> path; // Храним путь т.е. узел и индекс ребенка
        Node* current_node = root;
        size_t index = current_node->values.size() - 1; // Задаем индекс

        
        while ( !current_node->leaf ) { // Спускаемся по дереву, запоминая путь
   
            index = current_node->values.size() - 1;
            while ( index >= 0 && cmp( value, current_node->values[index] ) ) { // Ищем индекс ребенка
                index--;
            }
            index++; // Корректируем индекс ребенка

            if ( current_node->children[index]->values.size() == 2 * t - 1 ) { // Если ребенок заполнен
                split_child( current_node, index ); // Вызываем сплит
                
                if ( cmp( current_node->values[index], value ) ) { // После разделения возможно нужно выбрать другого ребенка (жестокое дз, то разделить ребенка, то другого выбрать)
                    index++;
                }
            }

          
            path.push( { current_node, index } );   // Сохраняем текущий узел и индекс для возможного обновления
            current_node = current_node->children[index];
        }

     
        index = current_node->values.size() - 1;
        current_node->values.resize(current_node->values.size() + 1);    // Обновляем размер

        while ( index >= 0 && cmp( value, current_node->values[index] ) ) { // Ищем место для вставки
            current_node->values[index + 1] = current_node->values[index];
            index--;
        }
        current_node->values[index + 1] = value; // И заполняем

    }

public:
    BTree(int minimal_order, Cmp cmp = Cmp()) : t(minimal_order), root(nullptr), cmp(cmp) {} // Констуктор по умолчанию

    ~BTree() { // Деструктор
        if (root) {
            delete root; // Корректен, тк внутри Node реализован местный деструктор
        }
    }

    BTree( const BTree & ) = delete; // Запрет на копирование
    BTree & operator = ( const BTree & ) = delete; // И присваивание

    void Add( const T& value ) { // Метод на добавление
        if ( !root ) { // Если дерево пустое
            root = new Node( true );
            root->values.push_back( value );
            return;
        }

       
        if ( root->values.size() == 2 * t - 1) {  // Если корень полон, разделяем его
            Node* new_root = new Node( false );
            new_root->children.push_back( root );
            root = new_root;
            split_child(root, 0);
        }

        add_non_full(root, value);
    }

    void print_by_levels() const { // Вывод данных по уровням
        if ( !root ) return; // Если дерево пустое

        std::queue<Node*> q; // Заводим очередь 
        q.push(root); // И помещаем туда корень дерева

        while ( !q.empty() ) { // До тех пор, пока не пройдем по всем элементам, цикл будет работать
            int level_size = q.size(); // Получаем размер текущего уровня
            for ( int i = 0; i < level_size; i++ ) {
                Node * current_node = q.front(); // Получаем первый элемент очереди
                q.pop();

          
                for ( const T & value : current_node->values ) { // Работаем с ключами текущего узла
                    std::cout << value;
                    if (&value != &current_node->values.back()) {
                        std::cout << " ";
                    }
                }

           
                for ( Node* child : current_node->children ) { // Добавляем всех детей в очередь
                    q.push( child );
                }

                // Разделитель между узлами в одном уровне
                if (i < level_size - 1) {
                    std::cout << " ";
                }

            }
            std::cout << std::endl;
        }
    }
};


struct IntCmp { // Компаратор для int
    bool operator()(int l, int r) const { return l < r; }
};



void run( std::istream & input, std::ostream & output ) {
    
    int t;
    input >> t; // Минимальный порядок

    IntCmp cmp;
    BTree<int, IntCmp> Tree(t, cmp); // Инициализация дерева

    int value;

    while ( input >> value ) { // Заполняем значения
        Tree.Add( value ); 
    }

    Tree.print_by_levels(); // Выводим по уровням
 
}

int main() {
    run( std::cin, std::cout );
    return 0;
}
