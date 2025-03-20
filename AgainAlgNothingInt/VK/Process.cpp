#include <iostream>
//В супермаркете решили оптимизировать показ рекламы.Известно расписание прихода и ухода покупателей(два целых числа).
//Каждому покупателю необходимо показать минимум 2 рекламы.Рекламу можно транслировать только в целочисленные моменты времени.
//Покупатель может видеть рекламу от момента прихода до момента ухода из магазина.
//В каждый момент времени может показываться только одна реклама.
//Считается, что реклама показывается мгновенно.Если реклама показывается в момент ухода или прихода, то считается, что посетитель успел её посмотреть.
//Требуется определить минимальное число показов рекламы.


struct Process { //Структура процесса
    int P; // Приоритет
    int t; // Время, которое уже отработал
    int T; // Общее время, необходимое для завершения

    Process( int p, int t, int T ) : P( p ), t(  t), T( T ) {}  // Конструктор

    Process() : P( 0 ), t( 0 ), T( 0 ) {}

};

template<typename T>
struct compareDefault{ // Компаратор по умолчанию
    bool operator()( const T & l, const T & r ) { return l < r; }
};

bool operator<( const Process & P1, const Process & P2 ){ // Оператор < для процесса
    return P1.P * ( P1.t + 1 ) > P2.P * ( P2.t + 1 );
}

template<typename T, typename compare = compareDefault<T>>
class BinaryHeap { // Реализация бинарной кучи
private:
    T * data; // Данные
    int capacity; // Вместительность
    int size; // Размер кучи
    compare cmp; // Компаратор

    void resize() { // Изменение размера динамического массива
        capacity *= 2;
        T * newData = new T[capacity];
         
        for ( int i = 0; i < size; i++ ) { // Перезапись данных
            newData[i] = data[i];
        }

        delete[] data; // Очищение данные

        data = newData;
    }

    void heapifyUp( int index ) { // Просеивание вверх
        while ( index > 0 ) {
            int parentIndex = ( index - 1 ) / 2;
            if (!cmp( data[index], data[parentIndex] ) ) {
                break;
            }
            std::swap( data[index], data[parentIndex] );
            index = parentIndex;
        }
    }

    void heapifyDown( int index ) { // Просеивание вниз
        // Инициализация индексов
        int leftChildIndex = 0;
        int rightChildIndex = 0;
        int smallestChildIndex = 0;

        while ( true ) { // Цикл работает до тех пор, кома самый маленький индекс не будет реавен искомому
            leftChildIndex = 2 * index + 1;
            rightChildIndex = 2 * index + 2;
            smallestChildIndex = index;

            if ( leftChildIndex < size && cmp( data[leftChildIndex], data[smallestChildIndex] ) ) { // wИспользуется переданный комапратор
                smallestChildIndex = leftChildIndex;
            }

            if ( rightChildIndex < size && cmp( data[rightChildIndex],data[smallestChildIndex] ) ) {
                smallestChildIndex = rightChildIndex;
            }

            if ( smallestChildIndex == index ) {
                break;
            }

            std::swap( data[index], data[smallestChildIndex] );
            index = smallestChildIndex;
        }

    }


public:
    BinaryHeap( int startCapacity = 10, compare comp = compareDefault<T>() ) 
        : capacity( startCapacity ), size( 0 ), cmp( comp ) {
        data = new T[capacity];
    }

    ~BinaryHeap() {
        delete[] data;
    }

    BinaryHeap( const BinaryHeap& ) = delete;

    BinaryHeap& operator=( const BinaryHeap& ) = delete;

    void insert( const T& value ) { // Операция вставки элемента в кучу
        if ( size >= capacity ) { // Увеличение размера при необходимости
            resize();
        }
        data[size] = value;

        heapifyUp( size ); // Просеивание вверх относительно нового значения
        size++;
    }

    T peek_min() { // Вовзращает минимальный элемент и удаляет его из кучи

        T min_el = data[0];
        data[0] = data[size - 1];
        size--;
        if ( size > 0 ) {
            heapifyDown(0);
        }
        return min_el;
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
    PriorityQueue( int startCapacity = 10) : heap(startCapacity ) {}

    void push( const T& value ) { // Вставка в очередь
        heap.insert( value );
    }

    T pop() {
        return heap.peek_min();
    }

    // Получение максимального элемента без удаления
    T top() const {

        return heap.get_min(); 
    }


    bool is_empty() const {
        return heap.is_empty();
    }

    int size() const {
        return heap.get_size();
    }
};

void run( std::istream& input, std::ostream& output ) {
    int n;
    input >> n; // Количество процессов

    PriorityQueue<Process> pq; // Очередь с приоритетом

    for ( int i = 0; i < n; i++ ) { // Ввод процессов
        int P, T;
        input >> P >> T;
        pq.push( Process( P, 0, T ) ); // Добавляем процесс с t = 0
    }

    int switchCount = 0; // Счетчик переключений

    
    while ( !pq.is_empty() ) { // Пока есть процессы в очереди
        Process current = pq.pop(); // Извлекаем процесс с минимальным P * (t + 1)
        switchCount++; // Увеличиваем счетчик переключений


        current.t += current.P; // Выполняем процесс на время P

    
        if ( current.t < current.T ) {     // Проверяем, завершен ли процесс
            pq.push(current);
        }
    }

    output << switchCount << std::endl;
}

int main() {
    run( std::cin, std::cout );
    return 0;

}
