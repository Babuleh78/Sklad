#include <iostream>
#include <vector>
#include <unordered_map>

using namespace std;

int main() {
    int n, b;
    cin >> n >> b;
    vector<int> a(n);
    int B_iter = -1;
    for (int i = 0; i < n; i++) {
        cin >> a[i];
        if (a[i] == b) {
            B_iter = i; //Последнее вхождение B
        }
    }

    unordered_map<int, int> Left_B; //разница между количеством элементов больше и меньше В
    int bal = 0;
    Left_B[bal] = 1; 
    //Лево В
    for (int i = B_iter - 1; i >= 0; i--) {
        if (a[i] > b) {
            bal++;
        }
        else if (a[i] < b) {
            bal--;
        }
        Left_B[bal]++;
    }

    bal = 0;
    int result = 0;
    //Право В
    for (int i = B_iter; i < n; i++) {
        if (a[i] > b) {
            bal++;
        }
        else if (a[i] < b) {
            bal--;
        }
        result += Left_B[-bal]; 
    }

    cout << result << endl;

    return 0;
}
