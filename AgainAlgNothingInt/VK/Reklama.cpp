#include <iostream>
//В супермаркете решили оптимизировать показ рекламы.Известно расписание прихода и ухода покупателей(два целых числа).
//Каждому покупателю необходимо показать минимум 2 рекламы.Рекламу можно транслировать только в целочисленные моменты времени.
//Покупатель может видеть рекламу от момента прихода до момента ухода из магазина.
//В каждый момент времени может показываться только одна реклама.
//Считается, что реклама показывается мгновенно.Если реклама показывается в момент ухода или прихода, то считается, что посетитель успел её посмотреть.
//Требуется определить минимальное число показов рекламы.


struct Visitor {
    int entryTime; // Время захода в магазин
    int exitTime; // Время выхода из магазина
};


bool operator<(const Visitor& v1, const Visitor& v2) { // Сравнение для сортировки по времени выхода
    if ( v1.exitTime < v2.exitTime ) {
        return true;
    }
    else if ( v1.exitTime == v2.exitTime ) {
        if ( v1.entryTime <= v2.entryTime ) {
            return true;
        }
     }
    return false;
}


template<typename T>
struct compareDefault {
    bool operator()( const T & l, const T & r) { return l < r; }
};

template<typename T, typename compare>
void merge( T* arr, int left, int mid, int right, compare cmp ) {
    // Вычисляем размеры временных массивов
    int n1 = mid - left + 1;
    int n2 = right - mid;

    // Создаем временные массивы Left L и Right R
    T* L = new T[n1];
    T* R = new T[n2];

    for (int i = 0; i < n1; i++) {
        L[i] = arr[left + i];
    }
    for (int j = 0; j < n2; j++) {
        R[j] = arr[mid + 1 + j];

    }

    //Объединение частей массива
    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (L[i] < R[j]) {
            arr[k] = L[i];
            i++;
        }
        else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    // Копируем оставшиеся элементы из Left
    while ( i < n1 ) {
        arr[k] = L[i];
        i++;
        k++;
    }

    // Копируем оставшиеся элементы Right
    while ( j < n2 ) {
        arr[k] = R[j];
        j++;
        k++;
    }

    // Освобождаем память
    delete[] L;
    delete[] R;
}

// Функция для сортировки массива покупателей
template<typename T, typename compare = compareDefault<T>>
void mergeSort( T * arr, int left, int right, compare cmp = compareDefault<T>() ) {
    if ( left < right ) {
        int mid = ( left + right ) / 2;
        mergeSort( arr, left, mid );
        mergeSort( arr, mid+1, right ) ;

        merge( arr, left, mid, right, cmp );
    }
}

void run( std::istream& input, std::ostream& output ) {
    int count = 0;//Итоговое количество рекламы

    int n; //Количество посетителей
    std::cin >> n;

    //Первое и последнее время показа рекламы, соответственно. secondAdTime > firstAdTime
    int firstAdTime = -1;
    int secondAdTime = -1;
    Visitor * vis = new Visitor[n]; //Массив посетителей

    for ( int i = 0; i < n; i++ ) { //Получение данных о посетителях
        int enter, exit;
        std::cin >> enter >> exit;

        vis[i].entryTime = enter;
        vis[i].exitTime = exit;
    }

    mergeSort( vis, 0, n - 1, compareDefault<Visitor>() ); //Сортировка массива

    for ( int i = 0; i < n; i++ ) {

        //Для каждого посетителя рассматриваем его время захода и выхода
        int entryTime = vis[i].entryTime;
        int exitTime = vis[i].exitTime;

        if (entryTime > secondAdTime) { //Если посетитель вошел позже показа последней рекламы
            count += 2; //Увеличиваем счетчик на два
            firstAdTime = exitTime - 1; //И "жадно" показываем рекламу
            secondAdTime = exitTime;
        }
        else if (entryTime == secondAdTime) { //Если время захода совпало с последней рекламой
            count++; //Показываем еще одну
            firstAdTime = secondAdTime; 
            secondAdTime = exitTime;
        }
        else if (entryTime > firstAdTime && exitTime > secondAdTime) { //Если посетитель застал вторую рекламу
            count++;
            firstAdTime = secondAdTime;
            secondAdTime = exitTime;
        }
        else if (entryTime > firstAdTime && entryTime < secondAdTime) { // Оставшийся случай
            count++;
            firstAdTime = exitTime;
        }
    }

    std::cout << count; //Выводим ответ

    delete[] vis; //Очищаем память
}

int main() {
    run(std::cin, std::cout);
    return 0;

}
