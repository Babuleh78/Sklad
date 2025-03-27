#include <iostream>
//Дан массив неотрицательных целых 64 - разрядных чисел.
//Количество чисел не больше 10^6.
//Отсортировать массив методом MSD по битам(бинарный QuickSort).

void binaryMSDSort( unsigned long long* arr, int left, int right, int bit_number ) {// Функция для бинарной сортировки

    if ( left >= right || bit_number < 0 ) { // Выход из функции при отсутствии необходимости сортировать
        return;
    }

    int left_copy = left; // Копии изначальных границ
    int right_copy = right;

    while( left <= right ) { 

        while ( left <= right && !( ( arr[left] >> bit_number ) & 1 ) ) { // Пока не встретим число с единицей на позиции bit_number сдвигаем левую границу
            left++;
        }

        while ( left <= right && ( ( arr[right] >> bit_number ) & 1 ) ) { // Пока не встретим число с нулем на позиции bit_number сдвигаем правую границу
            right--;
        }
        
        if ( left < right ) { // Если границы имеют смысл
            std::swap( arr[left], arr[right] ); // Меняем местами левый и правый элемент
            left++; 
            right--;
        }
    }

    binaryMSDSort( arr, left_copy, right, bit_number - 1 ); // Рекурсивно вызываем функцию для элементов с 0 на месте bit_number
    binaryMSDSort( arr, left, right_copy, bit_number - 1 ); // Рекурсивно вызываем функцию для элементов с 1 на месте bit_number
}


void sortBinaryMSD( unsigned long long * arr, int arr_size ) {
    const int max_bit = 63;
    binaryMSDSort( arr, 0, arr_size - 1, max_bit );
}

void run( std::istream& input, std::ostream& output ) {
    int n;
    input >> n; // Количество чисел

    unsigned long long* arr = new unsigned long long[n];
    for ( int i = 0; i < n; i++ ) {
        input >> arr[i];
    }
     
    sortBinaryMSD( arr, n );

    for ( int i = 0; i < n; i++ ) {
        output << arr[i] << " ";
    }

    delete[] arr;
 

  
} 

int main() {
    run( std::cin, std::cout );
    return 0;
}
