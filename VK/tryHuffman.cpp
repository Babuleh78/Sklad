#include <fstream>
#include <cstdio>
#include <vector>
#include <queue>
#include <unordered_map>
#include <bitset>
#include "Huffman.h"
#pragma warning(disable : 4996) 


// Узел дерева Хаффмана
struct HuffmanNode {
    byte value;
    int freq;
    HuffmanNode* left;
    HuffmanNode* right;

    HuffmanNode(byte val, int f) : value(val), freq(f), left(nullptr), right(nullptr) {}
    HuffmanNode(int f) : value(0), freq(f), left(nullptr), right(nullptr) {} 
};

struct Compare {
    bool operator()(HuffmanNode* a, HuffmanNode* b) {
        return a->freq > b->freq;
    }
};

class BitWriter {
private:
    IOutputStream& stream;
    byte buffer;
    int bitCount;

public:
    BitWriter(IOutputStream& os) : stream(os), buffer(0), bitCount(0) {}

    void WriteBit(bool bit) {
        if (bit) {
            buffer |= (1 << (7 - bitCount));
        }
        bitCount++;

        if (bitCount == 8) {
            stream.Write(buffer);
            buffer = 0;
            bitCount = 0;
        }
    }

    void Flush() {
        if (bitCount > 0) {
            stream.Write(buffer);
            buffer = 0;
            bitCount = 0;
        }
    }
};

class BitReader {
private:
    IInputStream& stream;
    byte buffer;
    int bitCount;
    bool eof;

public:
    BitReader(IInputStream& is) : stream(is), buffer(0), bitCount(8), eof(false) {}

    bool ReadBit(bool& bit) {
        if (bitCount == 8) {
            if (!stream.Read(buffer)) {
                eof = true;
                return false;
            }
            bitCount = 0;
        }

        bit = (buffer & (1 << (7 - bitCount))) != 0;
        bitCount++;
        return true;
    }

    bool IsEOF() const { return eof; }
};

HuffmanNode* BuildHuffmanTree(const std::unordered_map<byte, int>& freqMap) {
    std::priority_queue<HuffmanNode*, std::vector<HuffmanNode*>, Compare> pq;
    for (const auto& pair : freqMap) {
        pq.push(new HuffmanNode(pair.first, pair.second));
    }

    while (pq.size() > 1) {
        HuffmanNode* left = pq.top(); pq.pop();
        HuffmanNode* right = pq.top(); pq.pop();

        HuffmanNode* parent = new HuffmanNode(left->freq + right->freq);
        parent->left = left;
        parent->right = right;

        pq.push(parent);
    }

    return pq.top();
}

void BuildHuffmanCodes(HuffmanNode* root, const std::string& code, std::unordered_map<byte, std::string>& codes) {
    if (!root) return;

    if (!root->left && !root->right) {
        codes[root->value] = code;
        return;
    }

    BuildHuffmanCodes(root->left, code + "0", codes);
    BuildHuffmanCodes(root->right, code + "1", codes);
}

void WriteHuffmanTree(HuffmanNode* node, BitWriter& writer) {
    if (!node->left && !node->right) {
        writer.WriteBit(true); // лист
        for (int i = 7; i >= 0; --i) {
            writer.WriteBit((node->value >> i) & 1);
        }
    }
    else {
        writer.WriteBit(false); // внутренний узел
        WriteHuffmanTree(node->left, writer);
        WriteHuffmanTree(node->right, writer);
    }
}

HuffmanNode* ReadHuffmanTree(BitReader& reader) {
    bool bit;
    if (!reader.ReadBit(bit)) return nullptr;

    if (bit) { // лист
        byte value = 0;
        for (int i = 0; i < 8; ++i) {
            if (!reader.ReadBit(bit)) return nullptr;
            value = (value << 1) | (bit ? 1 : 0);
        }
        return new HuffmanNode(value, 0);
    }
    else { // внутренний узел
        HuffmanNode* left = ReadHuffmanTree(reader);
        HuffmanNode* right = ReadHuffmanTree(reader);
        HuffmanNode* node = new HuffmanNode(0);
        node->left = left;
        node->right = right;
        return node;
    }
}

void FreeHuffmanTree(HuffmanNode* node) {
    if (!node) return;
    FreeHuffmanTree(node->left);
    FreeHuffmanTree(node->right);
    delete node;
}

void Encode(IInputStream& original, IOutputStream& compressed) {
   
    std::unordered_map<byte, int> freqMap;
    std::FILE* tmp = std::tmpfile();
    byte value;
    while (original.Read(value)) {
        freqMap[value]++;
        std::fputc(value, tmp);
    }
    std::rewind(tmp);

    HuffmanNode* root = BuildHuffmanTree(freqMap);
    std::unordered_map<byte, std::string> codes;
    BuildHuffmanCodes(root, "", codes);

    BitWriter writer(compressed);

    WriteHuffmanTree(root, writer);
    writer.Flush();

    int c;
    while ((c = std::fgetc(tmp)) != EOF) {
        const std::string& code = codes[static_cast<byte>(c)];
        for (char bit : code) {
            writer.WriteBit(bit == '1');
        }
    }

    writer.Flush();
    FreeHuffmanTree(root);
    std::fclose(tmp);
}

void Decode(IInputStream& compressed, IOutputStream& original) {
    BitReader reader(compressed);

    HuffmanNode* root = ReadHuffmanTree(reader);
    if (!root) return;

    HuffmanNode* current = root;
    bool bit;

    while (reader.ReadBit(bit)) {
        if (bit) {
            current = current->right;
        }
        else {
            current = current->left;
        }

        if (!current->left && !current->right) {
            original.Write(current->value);
            current = root;
        }
    }

    FreeHuffmanTree(root);
}
