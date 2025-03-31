#include <iostream>
//
//Пара слов называется стековой анаграмой, если одно слово можно получить из другого,
//проведя последовательность стековых операций с его буквами(взять очередную букву
//    исходного слова и поместить ее в стек; взять букву из стека и добавить ее в конец
//    выходного слова).Для заданной пары слов требуется определить, можно ли выполнить
//    последовательность стековых операций, переводящую первое слово во второе.
//    Все буквы в слове различные.Длина анаграммы ≤ 10000.

template<typename T>
class Stack {
private:
    T * data; // Указатель на динамический массив
    int capacity; // Вместительность стека
    int top; // Индекс верхнего элемента стека

    void resize( int newCapacity ) {
        T* newData = new T[newCapacity]; // Инициализация нового массива

        for ( int i = 0; i < top; i++ ) {
            newData[i] = data[i]; // Копирование данных из старого массива
        }
        delete[] data; // Освобождение памяти

        capacity = newCapacity; // Переопределение переменных
        data = newData;
    }

public:
    Stack( int startCapacity = 10 ) : capacity( startCapacity ), top( 0 ) { // Создание стека с контролем вместительности
        if ( startCapacity <= 0 ) {
            throw std::invalid_argument( "Вместительность стека не может быть отрицательной!" );
        }
        data = new T[capacity];
    }

    ~Stack() {
        delete[] data;
    }



    Stack( const Stack& ) = delete;

    Stack& operator=( const Stack& ) = delete;

    int size() const { // Возвращает размер (число элементов стека)
        return top;
    }

    void push( const T& value ) {
        if ( top == capacity ) {
            resize( capacity * 2 ); // Увеличиваем размер вдвое в случае, когда вместительности не хватает
        }
        data[top] = value;
        top += 1;
    }

    void pop() { // Уменьшает индекс верхнего элемента
        if ( isEmpty() ) {
            throw std::out_of_range( "Стек пустой!" );
        }
        top--;
    }

    T peek() const {// Возвращает верхний элемент стека
        if ( isEmpty() ) {
            throw std::out_of_range( "Стек пустой!" );
        }
        return data[top - 1];
    }

    bool isEmpty() const {// Проверяет стек на пустоту
        return top == 0;
    }
};

bool isAnagram( char* word1, char* word2 ) {
    Stack<char> st;
    int i = 0, j = 0;

    while ( word1[i] != '\0' && word2[j] != '\0' ) { //Цикл идет пока не дошли до конца слова
        st.push( word1[i] ); // Помещаем в стек i-й жлемент первого слова
        i++;

        while ( !st.isEmpty() && st.peek() == word2[j] ) { // Пока в стеке есть символы из второго слова, делаем pop
            st.pop();
            j++;
        }
    }
     
    return st.isEmpty() && word1[i] == '\0' && word2[j] == '\0'; // True если стек пустой (все буквы использованы), а i и j указывают на конец слова
}

void run( std::istream& input, std::ostream& output ) {
    const int MAX_LEN = 10000;
    char word1[MAX_LEN];
    char word2[MAX_LEN];

    input >> word1 >> word2;

    if ( isAnagram( word1, word2) ) {
        output << "YES";
    }
    else {
        output << "NO";
    }
}

int main() {
    run(std::cin, std::cout);
    return 0;
}
