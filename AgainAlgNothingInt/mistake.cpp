#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
using namespace std;
int main() 
{
	int size;
    cin >> size;
    vector<int> a(size);
    vector<int> b(size);
    double all_case = 0;
    for(int i = 0;i <size; i++){
        int a_i, b_i;
        cin>>a_i; cin>>b_i;
        all_case+=a_i*b_i;
        b[i] = b_i; a[i] = a_i;
    }
    for(int i = 0; i<size; i++){
        double ver = a[i]*b[i];
        cout << fixed << setprecision(9) << ver/all_case << endl;
    }

	return 0;
}
