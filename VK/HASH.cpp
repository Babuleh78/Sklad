#include <iostream>
#include <string>

//Реализуйте структуру данных типа “множество строк” на основе динамической хеш - таблицы с
//открытой адресацией.Хранимые строки непустые и состоят из строчных латинских букв.
//Хеш - функция строки должна быть реализована с помощью вычисления значения многочлена методом Горнера.
//Начальный размер таблицы должен быть равным 8 - ми.Перехеширование выполняйте при добавлении
//элементов в случае, когда коэффициент заполнения таблицы достигает 3 / 4.
//Структура данных должна поддерживать операции добавления строки в множество, удаления строки
//из множества и проверки принадлежности данной строки множеству.
//1_2.Для разрешения коллизий используйте двойное хеширование.

const size_t DEFAULT_CAPACITY = 8; // Вместительность по умолчанию

class Hashtable { // Реализация хэш таблицы
private:

    struct HashCell { // Ячейка хэш таблицы, содержит:
        std::string value; // Значение 
        bool isBusy; // Занята ячейка или нет
        bool isDeleted; // Удалена ячейка или нет
    };

   
    HashCell* table; // Основная таблица, где будут храниться данные
    size_t capacity; // Вместительность таблицы
    size_t size; // Текущее количество элементов
    size_t prime1; // Простое число 1
    size_t prime2; // Простое число 2

    size_t h1( const std::string & key ) const { // Первая хэш функция
        size_t hash = 0;
        for ( char c : key ) {
            hash = ( hash * prime1 + c ); // Схема Горнера
        }
        return hash;
    }

    size_t h2( const std::string & key ) const { // Вторая хэш фнкция
        size_t hash = 0;
        for ( char c : key ) {
            hash = ( hash * prime2 + c );
        }
        hash = ( 2 * hash + 1 );
        return hash;
    }

    size_t Hashfunction( size_t hash1, size_t hash2, size_t i ) const { // Общая хэш функция
        return ( hash1 + i * hash2 ) % capacity;
    }

    

    void Rehash() {
        size_t oldCapacity = capacity; // Сохранение старой вместительность
        HashCell* oldTable = table; // И старой таблицы

        capacity *= 2;
        table = new HashCell[capacity]; // Инициализация новой таблицы

        for ( size_t i = 0; i < capacity; i++ ) { // Инициализация ячеек
            table[i].isBusy = false;
            table[i].isDeleted = false;
        }

        size = 0;

        for ( size_t i = 0; i < oldCapacity; i++ ) { // Заполнение ячеек
            if ( oldTable[i].isBusy && !oldTable[i].isDeleted ) {
                Add( oldTable[i].value );
            }
        }

        delete[] oldTable; // Очищение памяти
    }

public:
    Hashtable() : capacity( DEFAULT_CAPACITY ), size( 0 ), prime1( 71 ), prime2( 23 ) { // Конструктор
        table = new HashCell[capacity];

        for ( size_t i = 0; i < capacity; i++ ) { // Инициализация ячеек
            table[i].isBusy = false;
            table[i].isDeleted = false;
        }
    }

    ~Hashtable() {
        delete[] table; // Очищение памяти
    }

    // Запрещаем копирование и присваивание
    Hashtable( const Hashtable & ) = delete;
    Hashtable & operator=( const Hashtable & ) = delete;

    bool Add( const std::string & value ) {
        if ( size * 4 >= capacity * 3 ) { // 75% заполненность
            Rehash(); // Перехэширование
        }

        size_t hash1 = h1( value ); // Первый хэш
        size_t hash2 = h2( value ); // Второй хэш
        
        for ( size_t i = 0; i < capacity; i++ ) {
            size_t current_hash = Hashfunction( hash1, hash2, i ); // Вычисление текущего хэша

            if ( !table[current_hash].isBusy ) { // Если ячейка свободна
                table[current_hash].value = value; // Записываем значение
                table[current_hash].isBusy = true; // Занимаем ячейку
                table[current_hash].isDeleted = false; // Помечаем ее не удаленной
                size++; // Увеличиваем размер таблицы
                return true; 
            }

            if ( table[current_hash].isBusy && !table[current_hash].isDeleted && table[current_hash].value == value ) { // Если такое значение уже есть
                return false; 
            }

            if ( table[current_hash].isDeleted ) { // Если в ячейке занчение удалено

                bool exists = false;
                for ( size_t j = i + 1; j < capacity; j++ ) { // Убеждаемся, что дубликата не существует 
                    size_t next_hash = Hashfunction( hash1, hash2, j ); // Вычисление следующего хэша
                    if ( !table[next_hash].isBusy ) { // Если не занято
                        break;
                    }
                    if ( table[next_hash].isBusy && !table[next_hash].isDeleted && table[next_hash].value == value ) { // Если дубликат существует
                        exists = true;
                        break;
                    }
                } 
                if ( !exists ) { // Добавляем значение, если не существует
                    table[current_hash].value = value;
                    table[current_hash].isBusy = true;
                    table[current_hash].isDeleted = false;
                    size++;
                    return true;
                }
                else { // Иначе добавить не можем
                    return false;
                }
            }
        }

        return false;
    }

    bool Has( const std::string & value ) const { // Проверка наличия
        size_t hash1 = h1( value ); 
        size_t hash2 = h2( value );

        for ( size_t i = 0; i < capacity; i++ ) {
            size_t current_hash = Hashfunction( hash1, hash2, i ); // Вычисление текущего хэша

            if ( table[current_hash].isBusy && !table[current_hash].isDeleted && table[current_hash].value == value ) { // Если существует
                return true;
            }

            if ( !table[current_hash].isBusy ) { // Если ячейка с таким хэшем не занята
                break;
            }
        }

        return false;
    }

    bool Delete( const std::string & value ) { // Удаление
        size_t hash1 = h1( value );
        size_t hash2 = h2( value );

        for ( size_t i = 0; i < capacity; i++ ) {
            size_t current_hash = Hashfunction( hash1, hash2, i ); // Вычисление текущего хэша

            if ( table[current_hash].isBusy && !table[current_hash].isDeleted && table[current_hash].value == value ) { // Если существует
                table[current_hash].isDeleted = true; // "Удаляем" элемент
                size--; // Уменьшаем размер
                return true;
            }

            if ( !table[current_hash].isBusy ) { // Если ячейка с таким хэшем не занята
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
    bool Add( const std::string & value ) {
        return table.Add( value );
    }

    bool Delete( const std::string & value ) {
        return table.Delete( value );
    }

    bool Has( const std::string & value ) {
        return table.Has( value );
    }
};

void run( std::istream & input, std::ostream & output ) {
    StringSet test_set;

    char command;
    std::string value;

    while ( input >> command >> value ) {
        if ( command == '+' ) {
            test_set.Add( value ) ? output << "OK\n" : output << "FAIL\n";
        }
        else if ( command == '?' ) {
            test_set.Has( value ) ? output << "OK\n" : output << "FAIL\n";
        }
        else if( command == '-' ) {
            test_set.Delete( value ) ? output << "OK\n" : output << "FAIL\n";
        }
    }
}

int main() {
    run( std::cin, std::cout );
    return 0;
}
