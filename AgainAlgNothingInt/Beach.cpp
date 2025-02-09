#include <iostream>
#include <vector>
#include <algorithm> // для std::sort
using namespace std;

int main() 
{
    int T;
    cin >> T;
    
    while (T > 0) {
        int n;
        cin >> n;
        vector<int> vec(n);
        
        for (int i = 0; i < n; i++) {
            cin >> vec[i];
        }

        // Сначала сортируем массив
        sort(vec.begin(), vec.end());

        int minimum = 2147483647; 

        for (int i = 0; i < n - 1; i++) {
            minimum = min(minimum, vec[i] ^ vec[i + 1]);
        }

        cout << minimum << endl;

        T--;
    }

    return 0;
}
