class Solution {
public:
    int addDigits(int num) {
        if(num < 10){
            return num;
        } else{
            int s = 0;
            while(num>0){
                s+= num%10;
                num/=10;
            }
            return addDigits(s);
        }
        return num;
    }
};
