#include <iostream>
#include <vector>
#include <queue>

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
        priority_queue<int> max_heap; // Для меньшей половины
        priority_queue<int, vector<int>, greater<int>> min_heap; // Для большей половины

        for (int end = start; end < N; end++) {
            if (max_heap.empty() || A[end] <= max_heap.top()) {
                max_heap.push(A[end]);
            }
            else {
                min_heap.push(A[end]);
            }

            if (max_heap.size() > min_heap.size() + 1) {
                min_heap.push(max_heap.top());
                max_heap.pop();
            }
            else if (min_heap.size() > max_heap.size()) {
                max_heap.push(min_heap.top());
                min_heap.pop();
            }

            // Проверяем только подотрезки нечетной длины
            if ((end - start + 1) % 2 == 1) {
                if (max_heap.top() == B) {
                    count++;
                }
            }
        }
    }

    cout << count << endl;

    return 0;
}
