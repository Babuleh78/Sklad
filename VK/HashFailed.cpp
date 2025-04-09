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

    const size_t DEFAULT_SIZE = 8;


    template <typename T>
    struct HashCell {
        T value; // Значение ячейки
        bool isBusy = false; // Занята ли ячейка
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

    template <typename T, typename Hasher>
    class Hashtable {
    private:
   

        size_t size = 0; // Текущий размер
        size_t capacity; // Вместительность
        std::vector<HashCell<T>> table;
        Hasher hasher;

        size_t hash1(const T& key) const {
            return hasher(key, 23) % capacity;
        }

        size_t hash2(const T& key) const {
            return hasher(key, 97) % (capacity - 1) + 1;
        }

        void Rehash() {
            capacity *= 2;
            std::vector<HashCell<T>> new_table(capacity);
            std::swap(table, new_table);
            size = 0;

            for (const auto& cell : new_table) {
                if (cell.isBusy && !cell.isDeleted) {
                    Add(cell.value);
                }
            }

        }
    public:

        Hashtable(size_t initial_size = DEFAULT_SIZE, const Hasher& hasher = Hasher())
            : capacity(initial_size), hasher(hasher) {
            table.resize(capacity);
        }

        ~Hashtable() {

        }

        bool Add(const T& value) {
            if (size >= capacity * 0.75) { // alpha = size/capacity
                Rehash();
            }

            size_t h1 = hash1(value);
            size_t h2 = hash2(value);
            size_t i = 0;
            size_t first_deleted = capacity; // Невозможное значение

            while (i < capacity) {
                size_t index = (h1 + i * h2) % capacity;
                auto& cell = table[index];
                
                if (cell.isBusy && !cell.isDeleted && cell.value == value) {
                    return false; // Элемент уже существует
                }

                if (!cell.isBusy) {

                    if (first_deleted!=capacity) {
                        index = first_deleted;
                        cell = table[index];
                    }

                    cell.value = value;
                    cell.isBusy = true;
                    cell.isDeleted = false;
                    size++;

                    return true;
                }

                if (cell.isDeleted && first_deleted == capacity) {
                    first_deleted = index; // Запоминаем первую удалённую ячейку
                }
                i++;
            }

            if (first_deleted != capacity) {
                auto& cell = table[first_deleted];
                cell.value = value;
                cell.isBusy = true;
                cell.isDeleted = false;
                size++;
                return true;
            }

            return false;
        

        }

        bool Has(const T& value) {
            size_t h1 = hash1(value);
            size_t h2 = hash2(value);
            size_t i = 0;

            while (i < capacity) {
                size_t index = (h1 + h2 * i) % capacity;
                const auto& cell = table[index];

                if (!cell.isBusy) {
                    break;
                }

                if (cell.isBusy && !cell.isDeleted && cell.value == value) {
                    return true;
                }

                i++;
            }

            return false;
        }

        bool Delete(const T& value) {
            size_t h1 = hash1(value);
            size_t h2 = hash2(value);
            size_t i = 0;

            while (i < capacity) {
                size_t index = (h1 + h2 * i) % capacity;
                auto& cell = table[index];

                if (!cell.isBusy) {
                    return false;
                }

                if (cell.isBusy && !cell.isDeleted && cell.value == value) {
                    cell.isDeleted = true;
                    size--;
                    return true;
                }
                i++;
            }

            return false;
        }
    };

class StringSet {

    private:
        Hashtable<std::string, StringHasher> table;

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
            else {
                test_set.Delete(value) ? output << "OK\n" : output << "FAIL\n";
            }
        }
    }

    int main() {
        run( std::cin, std::cout );
        return 0;
    }
