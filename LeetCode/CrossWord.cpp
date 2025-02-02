#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
using namespace std;
int main() {
    int R, C;
    cin >> R >> C;

    vector<string> words;
    for (int i = 0; i < R; i++) {
        string word;
        cin >> word;
        words.push_back(word);
    }
    string min_word = "";

    // Проверка горизонтальных слов
    for (const auto& word : words) {
        if (word.length() >= 2 && word.find('#') == std::string::npos) {
            if (min_word.empty() || word < min_word) {
                min_word = word;
            }
        }
    }

    // Проверка вертикальных слов
    for (int col = 0; col < C; ++col) {
        string vertical_word;
        for (int row = 0; row < R; ++row) {
            vertical_word += words[row][col];
        }
        if (vertical_word.length() >= 2 && vertical_word.find('#') == std::string::npos) {
            if (min_word.empty() || vertical_word < min_word) {
                min_word = vertical_word;
            }
        }
    }

    if (!min_word.empty()) {
        cout << min_word << endl;
    }

    return 0;
}
