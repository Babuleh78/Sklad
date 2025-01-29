class Solution {
public:
    int findDuplicate(vector<int>& n) {
        for( int num : n){
            int idx = abs(num)-1;
            if(n[idx]<0){
                return abs(num);
            }
            n[idx]*=-1;
        }

        return -1;
    }
};
