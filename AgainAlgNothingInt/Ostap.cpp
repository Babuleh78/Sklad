#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;
int main() 
{
	int ans = 0;
	int pr, ku;
	cin >> pr; cin>> ku;
	vector<int> prod(pr);
	vector<int> kupi(ku);
	for(int i = 0; i<ku; i++){
		cin>>kupi[i];
	}
	sort(begin(kupi), end(kupi));
	for(int i = 0; i<pr; i++){
		cin>>prod[i];
	}
	sort(begin(prod), end(prod), greater<int>());
	int i = 0;
	while(i < pr && i < ku){
		if(kupi[i] - prod[i] >0){
			ans+=kupi[i] - prod[i];
		} else{
			break;
		}
		i++;
	}
	cout << ans;
	return 0;
}
