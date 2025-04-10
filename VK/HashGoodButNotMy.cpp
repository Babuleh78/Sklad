/*Задача 1. Хеш-таблица (8 баллов)
Реализуйте структуру данных типа “множество строк” на основе динамической хеш-таблицы с открытой адресацией. Хранимые строки непустые и состоят из строчных латинских букв. Начальный размер таблицы должен быть равным 8-ми. Перехеширование выполняйте при добавлении элементов в случае, когда коэффициент заполнения таблицы достигает 3/4.
Хеш-функцию строки реализуйте с помощью вычисления значения многочлена методом Горнера.
Структура данных должна поддерживать операции добавления строки в множество, удаления строки из множества и проверки принадлежности данной строки множеству.
1_1. Для разрешения коллизий используйте квадратичное пробирование. i-ая проба
g(k, i)=g(k, i-1) + i (mod m). m - степень двойки.
1_2. Для разрешения коллизий используйте двойное хеширование.
Формат входных данных
Каждая строка входных данных задает одну операцию над множеством. Запись операции состоит из типа операции и следующей за ним через пробел строки, над которой проводится операция.
Тип операции  – один из трех символов:
    +  означает добавление данной строки в множество; 
    -  означает удаление  строки из множества;  
    ?  означает проверку принадлежности данной строки множеству. 
При добавлении элемента в множество НЕ ГАРАНТИРУЕТСЯ, что он отсутствует в этом множестве. При удалении элемента из множества НЕ ГАРАНТИРУЕТСЯ, что он присутствует в этом множестве.
Формат выходных данных
Программа должна вывести для каждой операции одну из двух строк OK или FAIL, в зависимости от того, встречается ли данное слово в нашем множестве.
*/
 
#include <iostream>
#include <string>
#include <vector>
 
const int HashTableSize = 8;
 
using namespace std;
 
int Hash1( const string& key, int m ) {
    int hash = 0;
    int a = 11;
    for( int i = 0; i < static_cast<int>( key.size() ); i++ ) {
        hash = ( hash * a + key[i] ) % m;
    }
    return hash;
}
 
 
int Hash2( const string& key, int m ) {
    int hash = 0;
    int a = 17;
    for( int i = 0; i <static_cast<int>( key.size() ); i++ ) {
        hash = ( hash * a + key[i] ) % m;
    }
    hash =( 2 * hash + 1 ) % m;
    return hash;
}
 
 
int HashFunction( int h1, int h2, int i, int m ) {
    if( m != 0 ) {
        return( h1 + i * h2 ) % m;
    } else {
        return 0;
    }
}
 
struct HashTableNode {
    HashTableNode(const string &_key): key(_key), Del(false) {}
    string key;
    bool Del;
};
 
class CHashTable {
public:
    CHashTable(): table(HashTableSize, NULL), _size(0) {}
    bool Insert(const string &key);
    bool Remove(const string &key);
    bool Has(const string &key) const;
    void ReHash();
 
private:
    vector<HashTableNode*> table;
    int _size;
};
 
 
bool CHashTable::Insert( const string &key ) {
    if( static_cast<double>(_size)/static_cast<double>(static_cast<int>( table.size())) >=0.75) {
        ReHash();
    }
 
    int h1 = Hash1(key, static_cast<int>( table.size()) );
    int h2 = Hash2(key, static_cast<int>( table.size()) );
    int h = HashFunction(h1, h2, 0, static_cast<int>( table.size()));
    for (int i=0; i<static_cast<int>( table.size()) and table[h] != NULL; i++) {
        if(table[h]->key == key and table[h]->Del==false) {
            return false;
        }
        if(table[h]->Del==true ) {
            table[h]->key=key;
            table[h]->Del=false;
            _size++;
            return true;
        }
        h=HashFunction(h1, h2, i+1, static_cast<int>( table.size()));
    }
 
    table[h] = new HashTableNode(key);
    _size++;
    return true;
}
 
bool CHashTable::Remove(const string &key) {
    int h1 = Hash1(key, static_cast<int>( table.size()) );
    int h2 = Hash2(key, static_cast<int>( table.size()) );
    int j = HashFunction( h1, h2, 0, static_cast<int>( table.size()));
    for( int i = 0; i < static_cast<int>( table.size()); i++ ) {
        if(table[j] != NULL) {
            if( table[j]->key == key and table[j]->Del == false) {
                table[j]->Del = true;
                _size--;
                return true;
            }
        } else {
            return false;
        }
        j = HashFunction( h1, h2, i+1, static_cast<int>( table.size()));
    }
}
 
bool CHashTable::Has( const string &key ) const {
    int i = 0;
    int h1 = Hash1(key, static_cast<int>( table.size()) );
    int h2 = Hash2(key, static_cast<int>( table.size()) );
    int h=h1;
    while( table[h] != NULL and i < static_cast<int>( table.size()) ) {
        if( table[h]->key == key and table[h]->Del==false ){
            return true;
        }
        h = HashFunction( h1, h2, i+1, static_cast<int>( table.size()));
        i++;
    }
    return false;
}
 
void CHashTable::ReHash() {
    int newBufferSize = static_cast<int>( table.size()) * 2;
    vector<HashTableNode*> newBuffer(newBufferSize, NULL);
    for( int i = 0; i < static_cast<int>( table.size()); i++ ) {
        if( table[i] != NULL and table[i]->Del==false ) {
            int j = 0;
            int h1 = Hash1(table[i]->key, newBufferSize );
            int h2 = Hash2(table[i]->key, newBufferSize );
            int h = HashFunction(h1, h2, j, newBufferSize );
            while( j < newBufferSize ) {
                if (newBuffer[h] == NULL) {
                    break;
                }
                j++;
                h = HashFunction(h1, h2, j, newBufferSize );
            }
            newBuffer[h] = table[i];
        }
    }
    table = newBuffer;
}
 
int main()
{
    CHashTable table;
    while( true ) {
        char command = 0;
        string value;
        cin >> command >> value;
        if( cin.fail() )
        {
            break;
        }
        switch( command )
        {
        case '?':
            cout << ( table.Has( value ) ? "OK" : "FAIL" ) << std::endl;
            break;
        case '+':
            cout << ( table.Insert( value ) ? "OK" : "FAIL" ) << std::endl;
            break;
        case '-':
            cout << ( table.Remove( value ) ? "OK" : "FAIL" ) << std::endl;
            break;
        }
    }
 
 
    return 0;
}
