#include <iostream>
#include <limits>

int main() 
{
	int n;
    std::cin >> n;

    int maxMasha = -1;
    int minVasya = std::numeric_limits<int>::max();
    int sumVasya, sumMacsha = 0;

    for(int i = 1; i<=n; i++){
        int value;
        std::cin >> value;
        if(i%2 == 1){
            minVasya = std::min(minVasya, value);
            sumVasya+=value;
        }else{
            maxMasha = std::max(maxMasha, value);
            sumMacsha += value;
        }
    }


    if(maxMasha > minVasya){

        sumMacsha = sumMacsha + minVasya - maxMasha;
        sumVasya = sumVasya + maxMasha - minVasya;

    }


    std::cout << sumVasya - sumMacsha;
	return 0;
}
