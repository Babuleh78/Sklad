#include <iostream>
//В супермаркете решили оптимизировать показ рекламы.Известно расписание прихода и ухода покупателей(два целых числа).
//Каждому покупателю необходимо показать минимум 2 рекламы.Рекламу можно транслировать только в целочисленные моменты времени.
//Покупатель может видеть рекламу от момента прихода до момента ухода из магазина.
//В каждый момент времени может показываться только одна реклама.
//Считается, что реклама показывается мгновенно.Если реклама показывается в момент ухода или прихода, то считается, что посетитель успел её посмотреть.
//Требуется определить минимальное число показов рекламы.

template<typename T>
class BinaryHeap {
private:
    T * data; //Данные
    int capacity; //Вместительность
    int size; //Размер кучи

    void resize() {
        capacity *= 2;
        T * newData = new T[capacity];

        for ( int i = 0; i < size; i++ ) {
            newData[i] = data[i];
        }

        delete[] data;

        data = newData;
    }

    void heapifyUp(int index) {
        while (index > 0) {
            int parentIndex = ( index - 1 ) / 2;
            if (data[index] >= data[parentIndex]) {
                break;
            }
            std::swap(data[index], data[parentIndex]);
            index = parentIndex;
        }
    }

    void heapifyDown(int index) {
        int leftChildIndex = 0;
        int rightChildIndex = 0;
        int smallestChildIndex = 0;
        while (true) {
            leftChildIndex = 2 * index + 1;
            rightChildIndex = 2 * index + 2;
            smallestChildIndex = index;

            if (leftChildIndex < size && data[leftChildIndex] < data[smallestChildIndex]) {
                smallestChildIndex = leftChildIndex;
            }

            if (rightChildIndex < size && data[rightChildIndex] < data[smallestChildIndex]) {
                smallestChildIndex = rightChildIndex;
            }

            if (smallestChildIndex == index) {
                break;
            }

            std::swap(data[index], data[smallestChildIndex]);
            index = smallestChildIndex;
        }
        
    }


public:
    BinaryHeap(int startCapacity = 10)
        : capacity(startCapacity), size(0) {
        data = new T[capacity];
    }

    ~BinaryHeap() {
        delete[] data;
    }

    BinaryHeap( const BinaryHeap& ) = delete;

    BinaryHeap& operator=( const BinaryHeap& ) = delete;

    void insert(const T& value) {
        if (size >= capacity) {
            resize();
        }
        data[size] = value;
        heapifyUp(size);
        size++;
    }

    T peek_min() {
        
        T min_el = data[0];
        data[0] = data[size - 1];
        size--;
        if (size > 0) {
            heapifyDown(0);
        }
        return min_el;
    }

    T get_min() const{
        if (is_empty()) {
            return;
        }
        return data[0];
    }

    bool is_empty() const {
        return size == 0;
    }

    int get_size() const {
        return size;
    }
};


template<typename T>
class PriorityQueue {
private:
    BinaryHeap<T> heap; // Используем BinaryHeap как основу

public:
    PriorityQueue(int startCapacity = 10) : heap(startCapacity) {}

    void push(const T & value) {
        heap.insert(value);
    }

    T pop() {
        return heap.peek_min();
    }

    // Получение максимального элемента без удаления
    T top() const {
        
        return heap.get_min(); // В min-heap это минимальный элемент
    }


    bool is_empty() const {
        return heap.is_empty();
    }

    int size() const {
        return heap.get_size(); 
    }
};


struct Process {
    int P; // Приоритет
    int t; // Время, которое уже отработал
    int T; // Общее время, необходимое для завершения

    // Конструктор
    Process(int p, int t, int T) : P(p), t(t), T(T) {}

    Process() : P(0), t(0), T(0) {}

    // Оператор сравнения для кучи
    bool operator<(const Process& other) const {
        return P * (t + 1) > other.P * (other.t + 1); 
    }

    bool operator>=(const Process& other) const {
        return P * (t + 1) <= other.P * (other.t + 1); 
    }
};

void run( std::istream& input, std::ostream& output ) {
    int n;
    std::cin >> n; // Количество процессов

    PriorityQueue<Process> pq; // Очередь с приоритетом

    // Ввод процессов
    for (int i = 0; i < n; i++) {
        int P, T;
        std::cin >> P >> T;
        pq.push(Process(P, 0, T)); // Добавляем процесс с t = 0
    }

    int switchCount = 0; // Счетчик переключений

    // Пока есть процессы в очереди
    while (!pq.is_empty()) {
        Process current = pq.pop(); // Извлекаем процесс с минимальным P * (t + 1)
        switchCount++; // Увеличиваем счетчик переключений

        // Выполняем процесс на время P
        current.t += current.P;

        // Проверяем, завершен ли процесс
        if (current.t < current.T) {
            pq.push(current);
        }
    }

    std::cout << switchCount << std::endl;
}

int main() {
    run( std::cin, std::cout );
    return 0;

}
