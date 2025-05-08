#include <fstream>
#include <vector>
#include <iostream>
#include <queue>
#include <unordered_map>
#include <algorithm>
#include "Huffman.h"



// Узел дерева Хаффмана
struct HuffmanNode {
    byte value; // Значение (char)
    int freq; // Частоста
    HuffmanNode* left; // Левый потомок
    HuffmanNode* right; // Правый потомок

    HuffmanNode(byte sym, int f) : value(sym), freq(f), left(nullptr), right(nullptr) {} // Конструктор для листа
    HuffmanNode(int f, HuffmanNode* l, HuffmanNode* r) : value(0), freq(f), left(l), right(r) {} // Конструктор для узла

    ~HuffmanNode() { // Деструктор
        delete left;
        delete right;
    }
};

// в output Записываются данные, byte x - текущий "готовящийся" байт,
// byte i - счетчик записанных битов в текущем байте[0,7], byte bit - бит для записи
void writebit(IOutputStream& output, byte& x, byte& i, byte bit) // В пекло классы
{
    x = (x << 1) + bit; // Сдвигаем текущее значение на 1 и добавляем наш бит
    i += 1; // Увеличиваем счетчик записанных битов
    if (i == 8) // Если накопили байт
    {
        output.Write(x); // Записываем в выходящий поток
        i = 0; // И обнуляем счетчики
        x = 0;
    }
}
 
// из input получаем данные,  byte x - текущий "готовящийся" байт,
// i - счетчик записанных битов
byte readbit(IInputStream& input, byte& x, byte& i) // В пекло короля
{
    if (i == 8) // Если прочитали все биты
    {
        input.Read(x); // Читаем новый байт
        i = 0; // И обнуляем количество битов
    }
    i += 1;
    return (x >> (8 - i)) & 1; // Cдвигаем байт вправо так, чтобы нужный бит был в младшей позиции и явно его извлекаем (&1)
}

struct Compare {
    bool operator()(HuffmanNode* a, HuffmanNode* b) {
        return a->freq > b->freq;
    }
};


HuffmanNode* BuildHuffmanTree(const std::vector<std::pair<int, byte>>& freqMap) { // Функция для построения дерева Хаффмана
    auto cmp = Compare();

    std::priority_queue<HuffmanNode*, std::vector<HuffmanNode*>, decltype(cmp)> minHeap(cmp);

    for (const auto& entry : freqMap) {
        if (entry.first > 0) { // Для каждого символа с ненулевой частотой создается узел дерева
            minHeap.push(new HuffmanNode(entry.second, entry.first)); // Узлы добавляются в min-heap
        }
    }

    while (minHeap.size() > 1) {
        HuffmanNode* left = minHeap.top(); // Извлекаем два узла с наименьшими частотами
        minHeap.pop(); 
        HuffmanNode* right = minHeap.top();
        minHeap.pop();
        HuffmanNode* parent = new HuffmanNode(left->freq + right->freq, left, right); // Создается новый родительский узел, частота которого равна сумме частот дочерних узлов
        minHeap.push(parent); // Добавляем обратно в узел
    }

    return minHeap.empty() ? nullptr : minHeap.top();
}

//node - текущий узел, code - текущий код для символа, codes - коды для символов, куда будут записаны данные
void GenerateHuffmanCodes(HuffmanNode* node, const std::string& code, std::unordered_map<byte, std::string>& codes) { // Генерация кодов для дерева Хаффмана
    if (!node) return;

    if (!node->left && !node->right) { // Если текущая нода лист
        codes[node->value] = code; // Записываем текущий код к текущему символу
        return;
    }
    GenerateHuffmanCodes(node->left, code + "0", codes); // Рекурсивно продолжаем генерацию кодов
    GenerateHuffmanCodes(node->right, code + "1", codes);
}


void Encode(IInputStream& original, IOutputStream& compressed) {
    std::vector<std::pair<int, byte>> freqTable(256); // Для каждого символа
    std::vector<byte> buffer;

    byte x;
    int n = 0;
    while (original.Read(x)) { 
        ++n; 
        ++freqTable[x].first; // Увеличиваем частоту для символа x
        freqTable[x].second = x; // Сохраням наш символ
        buffer.push_back(x); // и сохраняем в буффер
    }

    std::sort(freqTable.begin(), freqTable.end()); // Сортируем символы по частоте
    HuffmanNode* root = BuildHuffmanTree(freqTable); // Для правильного построения дерефа Хаффмана (чем чаще символ, тем меньше у него код)

   
    std::unordered_map<byte, std::string> codes; 
    GenerateHuffmanCodes(root, "", codes); // Создаем коды

    
    for (int i = 0; i < 3; i++) { // Запишем длину исходных данных 
        compressed.Write(byte(n >> (16 - (i << 3)))); // Сдвигаем число и извлекаем нужный байт
    }

    byte uniq_symb = 0;
    for (const auto& entry : freqTable) { // Посчитаем количество уникальных символов
        if (entry.first > 0) {
            uniq_symb++;
        }
    }
    compressed.Write(uniq_symb - 1); // в m 1-256, надо записать 0-255

    byte tmp = 0, bitPos = 0; // буфер для накопления битов и счетчик битов в байте
    for (const auto& entry : freqTable) { // Запишем все коды для дешифратора
        if (entry.first > 0) { // Если встречается
            for (int i = 7; i >= 0; i--) { // Cначала записываем сам символ
                writebit(compressed, tmp, bitPos, (entry.second >> i) & 1);
            }

            const auto& code = codes[entry.second];
            for (int i = 7; i >= 0; i--) { // Потом длину кода, для экономии памяти и правильной работы дешифратора
                writebit(compressed, tmp, bitPos, (code.size() >> i) & 1);
            }
         
            for (char bit : code) { // Записываем сам код
                writebit(compressed, tmp, bitPos, bit == '1');
            }
        }
    }

    for (byte sym : buffer) { // Теперь запишем сами данные
        const std::string& code = codes[sym]; // Получаем код для каждого символа
        for (char bit : code) {
            writebit(compressed, tmp, bitPos, bit == '1'); // Побитно записываем символ
        }
    }

    while (bitPos != 0) { // Если не дошли до границы байта
        writebit(compressed, tmp, bitPos, 0); 
    }

    delete root;  // Освобождаем память
}   

void Decode(IInputStream& compressed, IOutputStream& original) {
    
    int data_len = 0; // количество символов в исходных данных
    for (int i = 0; i < 3; i++) { // Извлекаем размер
        byte b;
        compressed.Read(b);
        data_len += int(b) << (16 - (i << 3));
    }

    byte uniq_symb;
    compressed.Read(uniq_symb);
    uniq_symb += 1;  // Получаем количество уникальных символов

    HuffmanNode* root = new HuffmanNode(0, nullptr, nullptr);
    byte tmp = 0, bitPos = 8;

    for (int i = 0; i < uniq_symb; i++) { // Обрабатываем закодированные данные
 
        byte symbol = 0;
        for (int j = 0; j < 8; j++) { // Читаем символ
            symbol = (symbol << 1) | readbit(compressed, tmp, bitPos);
        }

        byte code_len = 0;
        for (int j = 0; j < 8; j++) { // Читаем длину кода
            code_len = (code_len << 1) | readbit(compressed, tmp, bitPos);
        }
      
        HuffmanNode* node = root; // Заводим ноду
        for (int j = 0; j < code_len; j++) {
            byte bit = readbit(compressed, tmp, bitPos); // Читаем бит
            if (bit) { // Если единичка
                if (!node->right) { // И нет правого потомка
                    node->right = new HuffmanNode(0, nullptr, nullptr); // Создаем новый узел
                }
                node = node->right; // смещаем узел вправо 
            }
            else { // Если же ноль, движемся влево
                if (!node->left) node->left = new HuffmanNode(0, nullptr, nullptr);
                node = node->left;
            }
        }
        node->value = symbol; // Записывем в ноду текущее значение
    }

    HuffmanNode* node = root;
    for (int i = 0; i < data_len; i++) { // Проходим по всем символам
        while (node->left || node->right) { // Пока узел не лист 
            byte bit = readbit(compressed, tmp, bitPos); // Считываем бит
            node = bit ? node->right : node->left; // Если 1, то сдвигаемся право, иначе влево
        }
        original.Write(node->value); // Записываем декодированный символ
        node = root; // Возвращаемся в корень
    }

    delete root;  // Освобождаем память
}
