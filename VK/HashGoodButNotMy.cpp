#include <iostream>
#include <vector>
//Реализуйте структуру данных типа “множество строк” на основе динамической хеш - таблицы
//с открытой адресацией.Хранимые строки непустые и состоят из строчных латинских букв.
//Хеш - функция строки должна быть реализована с помощью вычисления значения многочлена методом Горнера.
//Начальный размер таблицы должен быть равным 8 - ми.Перехеширование выполняйте
//при добавлении элементов в случае, когда коэффициент заполнения таблицы достигает 3 / 4.
//Структура данных должна поддерживать операции добавления строки в множество,
//удаления строки из множества и проверки принадлежности данной строки множеству.
//Для разрешения коллизий используйте двойное хеширование.

const size_t DEFAULT_CAPACITY = 8;

struct HashCell {
    std::string value;    // Значение ячейки
    bool isBusy = false;  // Занята ли ячейка
    bool isDeleted = false; // Удалена ли ячейка
};

struct StringHasher {
    size_t operator()(const std::string& str, size_t prime) const {
        size_t hash = 0;
        for (char c : str) {
            hash = hash * prime + c;
        }
        return hash;
    }
};

template <typename Hasher>
class Hashtable {
private:
    HashCell* table;      // Массив ячеек
    size_t capacity;      // Вместимость таблицы
    size_t size;          // Количество элементов
    size_t prime;         // Простое число для хеширования
    Hasher hasher;        // Функтор для вычисления хеша

    void initializeCells() {
        for (size_t i = 0; i < capacity; ++i) {
            table[i].isBusy = false;
            table[i].isDeleted = false;
        }
    }

    size_t h1(const std::string& str) const {
        return hasher(str, prime) % capacity;
    }

    size_t hash(const std::string& str, size_t i, size_t prevHash) const {
        (void)str; // Не используется, подавляем предупреждение
        return (prevHash + i) % capacity;
    }

    void Rehash() {
        size_t oldCapacity = capacity;
        HashCell* oldTable = table;

        capacity *= 2;
        table = new HashCell[capacity];
        initializeCells();
        size = 0;

        for (size_t i = 0; i < oldCapacity; ++i) {
            if (oldTable[i].isBusy && !oldTable[i].isDeleted) {
                Add(oldTable[i].value);
            }
        }

        delete[] oldTable;
    }

public:
    Hashtable() : capacity(DEFAULT_CAPACITY), size(0), prime(71) {
        table = new HashCell[capacity];
        initializeCells();
    }

    ~Hashtable() {
        delete[] table;
    }

    // Запрещаем копирование и присваивание
    Hashtable(const Hashtable&) = delete;
    Hashtable& operator=(const Hashtable&) = delete;

    bool Add(const std::string& value) {
        if (size * 4 >= capacity * 3) { // 75% заполненность
            Rehash();
        }

        size_t h = h1(value);
        size_t insertPos = -1;

        for (size_t i = 0; i < capacity; ++i) {
            h = hash(value, i, h);

            if (table[h].isBusy && !table[h].isDeleted && table[h].value == value) {
                return false; // Элемент уже существует
            }

            if (!table[h].isBusy) {
                if (insertPos == static_cast<size_t>(-1)) {
                    insertPos = h;
                }
                break;
            }

            if (table[h].isDeleted && insertPos == static_cast<size_t>(-1)) {
                insertPos = h;
            }
        }

        if (insertPos != static_cast<size_t>(-1)) {
            table[insertPos].value = value;
            table[insertPos].isBusy = true;
            table[insertPos].isDeleted = false;
            size++;
            return true;
        }

        return false;
    }

    bool Has(const std::string& value) const {
        size_t h = h1(value);

        for (size_t i = 0; i < capacity; ++i) {
            h = hash(value, i, h);

            if (!table[h].isBusy && !table[h].isDeleted) {
                return false; // Дошли до пустой ячейки
            }

            if (table[h].isBusy && !table[h].isDeleted && table[h].value == value) {
                return true;
            }
        }

        return false;
    }

    bool Delete(const std::string& value) {
        size_t h = h1(value);

        for (size_t i = 0; i < capacity; ++i) {
            h = hash(value, i, h);

            if (!table[h].isBusy && !table[h].isDeleted) {
                return false; // Дошли до пустой ячейки
            }

            if (table[h].isBusy && !table[h].isDeleted && table[h].value == value) {
                table[h].isDeleted = true;
                size--;
                return true;
            }
        }

        return false;
    }
};

class StringSet {

    private:
        Hashtable<StringHasher> table;

    public:
        bool Add(const std::string& value) {
            if (table.Has(value)) {
                return false;
            }

            return table.Add(value);
       
        }

        bool Delete(const std::string& value) {
            return table.Delete(value);
        }

        bool Has(const std::string& value) {
            return table.Has(value);
        }
    };

    void run(std::istream& input, std::ostream& output) {
        StringSet test_set;

        char command;
        std::string value;

        while (input >> command >> value) {
            if (command == '+') {
                test_set.Add(value) ? output << "OK\n" : output << "FAIL\n";
            }
            else if (command == '?') {
                test_set.Has(value) ? output << "OK\n" : output << "FAIL\n";
            }
            else if (command == '-'){
                test_set.Delete(value) ? output << "OK\n" : output << "FAIL\n";
            }
        }
    }

    int main() {
        run( std::cin, std::cout );
        return 0;
    }
