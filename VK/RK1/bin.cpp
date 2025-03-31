#include <iostream>

//Дан отсортированный по возрастанию массив попарно различных целых чисел A[0..n - 1].
//На вход также подается некоторое целое число k.Необходимо вернуть индекс элемента k 
//в массиве A, если он там присутствует, либо вернуть позицию, куда его следовало бы 
//вставить, чтобы массив остался отсортированным.
//
//Требование: сложность O(logN)

int searchPosition( int* arr, int k, int n ) {
    int left = 0;
    int right = n - 1;

    while ( left <= right ) { // Реализация бинарного поиска
        int mid = left + ( right - left ) / 2; // Вычисление среднего элемент

        if ( arr[mid] == k ) { // Если нашли искомый элемент
            return mid;
        }
        else if ( arr[mid] < k ) { // Если центральный элемент меньше k сдвигаем левую границу
            left = mid + 1;
        }
        else {
            right = mid - 1;  // Если центральный элемент меньше k сдвигаем правую границу
        }
    }

    return left; // В переменной left будет записана позиция, куда нужно вставить k, чтобы массив оставался отсортированным
}

void run( std::istream& input, std::ostream& output ) {

    int n; 
    input >> n; // Вводится размер массива
    
    int * arr = new int[n];
    for ( int i = 0; i < n; i++ ) { // Массива заполянется
        input >> arr[i];
    }

    int k;
    input >> k; // Вводится число, для которого необходимо найти позицию

    int result = searchPosition( arr, k, n );
    output << result;

    delete[] arr; // Очищение памяти
}

int main() {
    
    run( std::cin, std::cout );
    return 0;
}
