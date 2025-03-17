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
    return v1.exitTime < v2.exitTime;
}

bool operator<=(const Visitor& v1, const Visitor& v2) { // Сравнение для сортировки по времени выхода
    return v1.exitTime <= v2.exitTime;
}



std::ostream& operator<< (std::ostream& os, const Visitor& vis) {
    os << vis.entryTime << " " << vis.exitTime << " | ";
    return os;
}

template<typename T>
struct compareDefault {
    bool operator()(const T& l, const T& r) { return l < r; }
};

template<typename T, typename compare>
void merge(T* arr, int left, int mid, int right, compare cmp) {
    // Вычисляем размеры временных массивов
    int n1 = mid - left + 1;
    int n2 = right - mid;

    // Создаем временные массивы
    T* L = new T[n1];
    T* R = new T[n2];

    // Копируем данные во временные массивы L[] и R[]
    for (int i = 0; i < n1; i++)
        L[i] = arr[left + i];
    for (int j = 0; j < n2; j++)
        R[j] = arr[mid + 1 + j];

    // Слияние временных массивов обратно в arr[left..right]
    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        }
        else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    // Копируем оставшиеся элементы L[], если они есть
    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }

    // Копируем оставшиеся элементы R[], если они есть
    while (j < n2) {
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
void mergeSort(T* arr, int left, int right, compare cmp = compareDefault<T>()) {
    if (left < right) {
        int mid = (left + right) / 2;
        mergeSort(arr, left, mid);
        mergeSort(arr, mid+1, right);

        merge(arr, left, mid, right, cmp);
    }
}

int main() {
    int count = 0;
    int n;
    std::cin >> n;
    int adTime0 = -1;
    int adTime1 = -1;
    Visitor* vis = new Visitor[n];

    for (int i = 0; i < n; i++) {
        int enter, exit;
        std::cin >> enter >> exit;
        vis[i].entryTime = enter;
        vis[i].exitTime = exit;
    }
    mergeSort(vis, 0, n-1, compareDefault<Visitor>());

    for (int i = 0; i < n; i++) {
        int entry = vis[i].entryTime;
        int exit = vis[i].exitTime;

        if (adTime0 < entry) {
            adTime0 = exit;
            count++;
        }

        if (adTime1 < entry) {
            adTime1 = exit - 1;
            count++;
        }
    }
    std::cout << count;

    delete[] vis;
    return 0;

    
}
