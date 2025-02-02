#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>

using namespace std;

int main() {
    int r, c;
    cin >> r >> c;
    cin.ignore();

    vector<string> crossword(r);
    for (int i = 0; i < r; ++i) {
        getline(cin, crossword[i]);
    }

    vector<string> allWords;
    for (int i = 0; i < r; ++i) {
        stringstream ss(crossword[i]);
        string word;

        while (getline(ss, word, '#')) {
            if (word.length() >= 2) {
                allWords.push_back(word);
            }
        }
    }

    for (int j = 0; j < c; ++j) {
        string colWord;
        for (int i = 0; i < r; ++i) {
            char ch = crossword[i][j];

            if (ch == '#') {
                if (colWord.length() >= 2) {
                    allWords.push_back(colWord);
                }
                colWord.clear();
            }
            else {
                colWord += ch;
            }
        }

        if (colWord.length() >= 2) {
            allWords.push_back(colWord);
        }
    }

    sort(allWords.begin(), allWords.end());



    cout << allWords[0] << endl;


    return 0;
}
