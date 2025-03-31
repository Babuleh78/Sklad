#include <iostream>

//На склад привезли много пустых ящиков.Все ящики пронумерованы по порядку поступления от 0.
//Известно, что их все можно сложить один в один(то есть так, что каждый следующий помещается 
//в предыдущий).Один ящик можно вложить в другой, если его можно перевернуть так, что 
// размеры одного ящика по всем осям станут строго меньше размеров другого ящика по
//соответствующим осям. Требуется определить, в какой последовательности они будут вложены друг в друга.
//Вывести номера ящиков.

struct Box {
    long long capacity = 0; // Вместительность
    int number = 0; // Номер для вывода
};

void insertionSort(Box* arr, int length) { // Реализация сортировки вставками
    for ( int i = 1; i < length; i++ ) { 

        Box temp = arr[i];
        int j = i - 1;
        while ( j >= 0 && temp.capacity < arr[j].capacity ) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = temp;
    }
}

void run( std::istream& input, std::ostream& output ) {
    int n = 0;
    input >> n;
    Box* arr = new Box[n];

    for ( int i = 0; i < n; i++ ) { // Заполнение массива коробок
        int a, b, c;
        input >> a >> b >> c;

        Box currentBox; // Текущая коробка
        currentBox.capacity = a * b * c;
        currentBox.number = i; // Нумеруем коробку

        arr[i] = currentBox;
    }

    insertionSort(arr, n); // Сортируем массив


    for (int i = 0; i < n; i++)
        output << arr[i].number << " ";

    delete[] arr; // Очищение памяти

}

int main() {
    run( std::cin, std::cout );
    return 0;
}
