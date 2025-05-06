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
}
