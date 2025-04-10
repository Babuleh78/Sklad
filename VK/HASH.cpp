#include <iostream>
#include <vector>
#include <string>

const size_t DEFAULT_CAPACITY = 8;

class Hashtable {
private:
    struct HashCell {
        std::string value;
        bool isBusy;
        bool isDeleted;
    };

    static const size_t DEFAULT_CAPACITY = 8;
    HashCell* table;
    size_t capacity;
    size_t size;
    size_t prime1;
    size_t prime2;

    size_t h1(const std::string& key) const {
        size_t hash = 0;
        size_t a = 11;
        for (char c : key) {
            hash = (hash * a + c) % capacity;
        }
        return hash;
    }

    size_t h2(const std::string& key) const {
        size_t hash = 0;
        size_t a = 17;
        for (char c : key) {
            hash = (hash * a + c) % capacity;
        }
        hash = (2 * hash + 1) % capacity;
        return hash;
    }

    size_t Hashfunction(size_t hash1, size_t hash2, size_t i) const {
        return (hash1 + i * hash2) % capacity;
    }

    void initializeCells() {
        for (size_t i = 0; i < capacity; ++i) {
            table[i].isBusy = false;
            table[i].isDeleted = false;
        }
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
    Hashtable() : capacity(DEFAULT_CAPACITY), size(0), prime1(71), prime2(61) {
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

        size_t hash1 = h1(value);
        size_t hash2 = h2(value);

        for (size_t i = 0; i < capacity; ++i) {
            size_t current_hash = Hashfunction(hash1, hash2, i);

            if (!table[current_hash].isBusy) {
                table[current_hash].value = value;
                table[current_hash].isBusy = true;
                table[current_hash].isDeleted = false;
                size++;
                return true;
            }

            if (table[current_hash].isBusy && !table[current_hash].isDeleted &&
                table[current_hash].value == value) {
                return false;
            }

            if (table[current_hash].isDeleted) {
                // Check if the same value exists later in the probe sequence
                bool exists = false;
                for (size_t j = i + 1; j < capacity; ++j) {
                    size_t next_hash = Hashfunction(hash1, hash2, j);
                    if (!table[next_hash].isBusy) break;
                    if (table[next_hash].isBusy && !table[next_hash].isDeleted &&
                        table[next_hash].value == value) {
                        exists = true;
                        break;
                    }
                }
                if (!exists) {
                    table[current_hash].value = value;
                    table[current_hash].isBusy = true;
                    table[current_hash].isDeleted = false;
                    size++;
                    return true;
                }
                else {
                    return false;
                }
            }
        }

        return false;
    }

    bool Has(const std::string& value) const {
        size_t hash1 = h1(value);
        size_t hash2 = h2(value);

        for (size_t i = 0; i < capacity; ++i) {
            size_t current_hash = Hashfunction(hash1, hash2, i);

            if (table[current_hash].isBusy && !table[current_hash].isDeleted &&
                table[current_hash].value == value) {
                return true;
            }

            if (!table[current_hash].isBusy) {
                break;
            }
        }

        return false;
    }

    bool Delete(const std::string& value) {
        size_t hash1 = h1(value);
        size_t hash2 = h2(value);

        for (size_t i = 0; i < capacity; ++i) {
            size_t current_hash = Hashfunction(hash1, hash2, i);

            if (table[current_hash].isBusy && !table[current_hash].isDeleted &&
                table[current_hash].value == value) {
                table[current_hash].isDeleted = true;
                size--;
                return true;
            }

            if (!table[current_hash].isBusy) {
                break;
            }
        }

        return false;
    }
};


class StringSet {
private:
    Hashtable table;

public:
    bool Add(const std::string& value) {
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
        else if (command == '-') {
            test_set.Delete(value) ? output << "OK\n" : output << "FAIL\n";
        }
    }
}

int main() {
    run(std::cin, std::cout);
    return 0;
}
