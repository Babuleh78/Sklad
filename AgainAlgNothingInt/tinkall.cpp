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




#include <iostream>
#include <string>
#include <vector>
#include <unordered_set>
using namespace std;




int main() {
    int line_count;
    cin >> line_count;
    vector<pair<int, int>> metro_vector = vector<pair<int, int>>(line_count);

    for (int i = 0; i < line_count; i++) {
        pair<int, int> value;
        cin >> value.first >> value.second;
        metro_vector[i] = value;
    }

    int query_count;
    cin >> query_count;
    
    int metro_line = 0;
    int time = 0;
    for (int i = 0; i < query_count; i++) {
        cin >> metro_line >> time;

        int first_train = metro_vector[metro_line - 1].first;
        int interval = metro_vector[metro_line - 1].second;

        int current_time;
        if (time < first_train) {
            current_time = first_train - time;
        }
        else {
            int trains_passed = (time - first_train) / interval;
            
            int next_train_time = first_train + (trains_passed)*interval;
            if (next_train_time == time) {
                current_time = next_train_time;
            }
            else {
                current_time = next_train_time + interval;
            }
            
        }

        cout << current_time << endl;

    }

    return 0;



#include <iostream>
#include <vector>
#include <queue>
#include <list>
#include <algorithm>

using namespace std;

struct Compare {
    bool operator()(const pair<int, int>& a, const pair<int, int>& b) {
        if (a.first == b.first) {
            return a.second > b.second;  
        }
        return a.first < b.first;  
    }
};


long long maxTotalDifference(int n, vector<int>& a) {
    if (n == 2) return abs(a[0] - a[1]);

    vector<int> diffs(n - 1);
    for (int i = 0; i < n - 1; ++i) {
        diffs[i] = abs(a[i] - a[i + 1]);
    }
    priority_queue<pair<int, int>, vector<pair<int, int>>, Compare> max_heap;

    for (int i = 0; i < n - 1; ++i) {
        max_heap.push({ diffs[i], i });
    }

    int size = a.size();
    vector<bool> is_active(n - 1, true);
    long long total = 0;

    while (!max_heap.empty() && size>=2) {
        auto current = max_heap.top();
        max_heap.pop();
        int diff = current.first;
        int index = current.second;

        if (!is_active[index]) continue;

        total += diff;
        is_active[index] = false;

        int left = index;
        while (left >= 0 && !is_active[left]) {
            left--;
        }

        int right = index;
        while (right < n - 1 && !is_active[right]) {
            right++;
        }

        if (left >= 0 && right < n - 1) {
            int new_diff = abs(a[left + 1] - a[right]);
            diffs[left] = new_diff;
            max_heap.push({ new_diff, left });
            is_active[right] = false; 
        }

        size -= 2;
    }

    return total;
}

int main() {
    int n;
    cin >> n;
    vector<int> a(n);
    for (int i = 0; i < n; i++) {
        cin >> a[i];
    }

    cout << maxTotalDifference(n, a) << endl;

    return 0;
}
}
