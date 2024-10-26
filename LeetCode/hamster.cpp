class Solution {
public:
    int addDigits(int num) {
        while(num>=10){
            num = helper(num);
        }
        return num;
    }

    int helper(int n){
        int s = 0;
        while(n>0){
            s+=n%10;
            n = n/10;
        }
        return s;
    }
};
