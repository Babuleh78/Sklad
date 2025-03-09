// 3.3 Реализовать очередь с помощью двух стеков.
// Требования: Очередь должна быть реализована в виде класса.
// Стек тоже должен быть реализован в виде класса (на основе динамического массива).
//a = 1 - push front
//a = 2 - pop front
//a = 3 - push back
//a = 4 - pop back

#include <iostream>
template<typename T>
class Stack {
private:
    T* data; // Указатель на динамический массив
    int capacity; // Вместительность стека
    int top; // Индекс верхнего элемента стека

    void resize(int newCapacity) {
        T* newData = new T[newCapacity]; // Инициализация нового массива

        for (int i = 0; i < top; i++) {
            newData[i] = data[i]; // Копирование данных из старого массива
        }
        delete[] data; // Освобождение памяти

        capacity = newCapacity; // Переопределение переменных
        data = newData;
    }

public:
    Stack(int startCapacity = 10) : capacity(startCapacity), top(0) { // Создание стека с контролем вместительности
        if (startCapacity <= 0) {
            throw std::invalid_argument("Вместительность стека не может быть отрицательной!");
        }
        data = new T[capacity];
    }

    ~Stack() {
        delete[] data;
    }

    

    Stack(const Stack&) = delete;

    Stack& operator=(const Stack&) = delete;

    int size() const { // Возвращает размер (число элементов стека)
        return top;
    }

    void push(const T& value) {
        if (top == capacity) {
            resize(capacity * 2); // Увеличиваем размер вдвое в случае, когда вместительности не хватает
        }
        data[top] = value;
        top += 1;
    }

    void pop() { //уменьшает индекс верхнего элемента
        if (isEmpty()) {
            throw std::out_of_range("Стек пустой!");
        }
        top--;
    }

    T peek() const {//возвращает верхний элемент стека
        if (isEmpty()) {
            throw std::out_of_range("Стек пустой!");
        }
        return data[top - 1];
    }

    bool isEmpty() const {//проверяет стек на пустоту
        return top == 0;
    }
};
template<typename T>
class Queue {
private:
    Stack<T> stack_for_add; // Стек для добавления элементов
    Stack<T> stack_for_del; // Стек для удаления элементов

    void transfer() {   // Переносим элементы из stack_for_add в stack_for_del, если stack_for_del пуст
      
        if (stack_for_del.isEmpty()) {
            while (!stack_for_add.isEmpty()) {
                stack_for_del.push(stack_for_add.peek());
                stack_for_add.pop();
            }
        }
    }

public:
   

    void push_back(int value) {  // Добавление элемента в конец очереди
        stack_for_add.push(value);
    }

   
    void push_front(int value) { // Добавление элемента в начало очереди
        stack_for_del.push(value);
    }

   
    int pop_front() {  // Удаление элемента из начала очереди
        transfer();
        if (stack_for_del.isEmpty()) {
            return -1; // Возвращаем -1, если очередь пуста для проверки
        }
        int value = stack_for_del.peek();//Сохраняем удаленное значение
        stack_for_del.pop();
        return value;//Возвращаем удаленное значение
    }

   
    int pop_back() { // Удаление элемента из конца очереди
        transfer();
        if (stack_for_add.isEmpty()) {
            return -1; // Возвращаем -1, если очередь пуста
        }
        int value = stack_for_add.peek();//Сохраняем удаленное значение
        stack_for_add.pop();
        return value;//Возвращаем удаленное значение
    }

 
    bool is_empty() const {   // Проверка, пуста ли очередь
        return stack_for_add.empty() && stack_for_del.empty();
    }
};

void run( std::istream& input, std::ostream& output ) {
    int command_count;
    Queue<int> queue;
    std::cin >> command_count;

    int* results = new int[command_count];//Массив результатов работы
    int* expected_results = new int[command_count];//Ожидаемые результаты работы

    for (int i = 0; i < command_count; i++) {
        int command_code;//код команды
        int value;//искомое значение

        std::cin >> command_code;
        std::cin >> value;

        switch (command_code) {//Действия в зависимости от кода команды
        case 1:
            queue.push_front(value);
            break;
        case 2:
            expected_results[i] = value;
            results[i] = queue.pop_front();
            break;
        case 3:
            queue.push_back(value);
            break;
        case 4:
            expected_results[i] = value;
            results[i] = queue.pop_back();
            break;
        }
      
    }

    bool flag = true;
    for (int i = 0; i < command_count; i++) {
        if (results[i] != expected_results[i]) {//поиск несоответствий в работе
            flag = false;
            break;
        }
    }

    if (flag) {//Вывод результата
        std::cout << "YES";
    }
    else {
        std::cout << "NO";
    }

    delete[] results;//очищение памяти
    delete[] expected_results;
}
int main() {
    run ( std::cin, std::cout );
    return 0;
}
