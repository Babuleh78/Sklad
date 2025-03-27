#include <iostream>
#include <cstdlib> // Для функции rand()
//Дано множество целых чисел из[0..10 ^ 9] размера n.
//Используя алгоритм поиска k - ой порядковой статистики, требуется найти следующие параметры множества :
//10 % перцентиль
//медиана
//90 % перцентиль
//
//Реализуйте стратегию выбора опорного элемента “случайный элемент”.
//Функцию Partition реализуйте методом прохода двумя итераторами от начала массива к концу.
template<typename T>
struct compareDefault { // Компаратор по умолчанию
    bool operator()( const T & l, const T & r ) { return l < r; }
};


template <typename T>
void swap( T& a, T& b ) { // Реализация шаблонной функции swap
    T temp = a;
    a = b;
    b = temp;
}

template<typename T, typename compare = compareDefault<T>>
int Partition( T * arr, int left, int right, compare cmp = compare() ) {
    int pivotIndex = left + rand() % ( right - left + 1 ); // Определение индекса цели
    T pivot = arr[pivotIndex];
    swap( arr[pivotIndex], arr[right] ); // Перемещаем pivot в конец массива

    int i = left; // Указатель на левую границу 
    for ( int j = left; j < right; j++ ) {
        if ( cmp( arr[j], pivot ) ) { // Сравниваем при помощи переданного компаратора 
            swap( arr[i], arr[j] ); // Помещаем его в левую часть
            i++; // Сдвигаем левую границу
        }
    }
    swap( arr[i], arr[right] ); // Возвращаем pivot на его правильную позицию 

    return i; // Возвращаем индекс pivot-a
}

template<typename T, typename compare = compareDefault<T>>
T QuickSelect( T * arr, int left, int right, int k, compare cmp = compare() ) {

    while ( left < right ) { // Продолжаем поиск, пока границы не сойдутся
        int pivotIndex = Partition( arr, left, right, cmp );

        if ( k == pivotIndex ) { // Если pivot оказался на искомой позиции - возвращаем его
            return arr[k];
        }
        else if ( k < pivotIndex ) {
            right = pivotIndex - 1;
        }
        else {
            left = pivotIndex + 1;
        }
    }
    return arr[left]; // Если left == right значит мы сузили поиск до одного элемента
}

template<typename T, typename compare = compareDefault<T>>
void run( std::istream& input, std::ostream& output, compare cmp = compareDefault<T>() ){

    int n; // Количество чисел
    input >> n; 
    T* arr = new T[n]; // Инициализация динамического массива

    for ( int i = 0; i < n; i++ ) { // Заполнение массива
        input >> arr[i];
    }

    int index10 = (10 * n) / 100;  // Находим индексы перцентилей
    int index50 = (50 * n) / 100;
    int index90 = (90 * n) / 100;

    index10 = std::max(0, std::min(index10, n - 1)); // Гарантируем, что индексы в пределах массива
    index50 = std::max(0, std::min(index50, n - 1));
    index90 = std::max(0, std::min(index90, n - 1));

    T p10 = QuickSelect(arr, 0, n - 1, index10, cmp);  // Ищем перцентили с помощью QuickSelect
    T p50 = QuickSelect(arr, 0, n - 1, index50, cmp);
    T p90 = QuickSelect(arr, 0, n - 1, index90, cmp);

    output << p10 << "\n";  // 10-й перцентиль
    output << p50 << "\n";  // Медиана 50-й перцентиль
    output << p90 << "\n";  // 90-й перцентиль

    delete[] arr; // Очищение памяти
}


int main() {
    run<int>( std::cin, std::cout );
    return 0;
}
