#include <iostream>
#include <vector>
#include <set>

using namespace std;

int main() {
    int N, B;
    cin >> N >> B;

    vector<int> A(N);
    for (int i = 0; i < N; i++) {
        cin >> A[i];
    }

    int count = 0;

    for (int start = 0; start < N; start++) {
        multiset<int> med_set; // Для хранения текущего подотрезка
        for (int end = start; end < N; end++) {
            med_set.insert(A[end]); 

            // Проверяем только подотрезки нечетной длины
            if ((end - start + 1) % 2 == 1) {
                // Находим медиану 
                int mid_index = (end - start) / 2;
                auto median_it = next(med_set.begin(), mid_index); 
                if (*median_it == B) {
                    count++;
                }
            }
        }
    }

    cout << count << endl;

    return 0;
}
