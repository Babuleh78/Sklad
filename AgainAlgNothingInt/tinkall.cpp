#include <iostream>
#include <string>
#include <vector>
#include <unordered_set>
using namespace std;

int countElements(vector<int>& vec) {
    unordered_set<int> uniqueElements;
    uniqueElements.insert(0);

    for (int num : vec) {
        while (num > 0) {
            uniqueElements.insert(num);
            if (uniqueElements.size() >= vec.size()) {
                return uniqueElements.size();
            }
            num /= 2; 
        }
    }


    return uniqueElements.size();
}


int main() {
    int size;
    cin >> size;
    vector<int> vec = vector<int>(size);

    for (int i = 0; i < size; i++) {
        cin >> vec[i]; 
    }

    cout << countElements(vec) << endl;

    return 0;
}
