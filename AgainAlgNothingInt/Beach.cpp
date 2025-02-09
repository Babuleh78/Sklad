#include <iostream>
#include <string>
#include <vector>
using namespace std;

int main() 
{
	int T;
	cin >> T;
	while(T>0){
		int n;
		cin >> n;
		vector<int> vec(n);
		for(int i = 0; i<n; i++){
			cin>>vec[i];
		}
		int minimum = 100000;
		for(int i = 0; i<n; i++){
			for(int j = i+1; j<n; j++){
				minimum = min(minimum, vec[i]^vec[j]);
			}
		}

		cout << minimum;

		T--;

	}

	return 0;
}
