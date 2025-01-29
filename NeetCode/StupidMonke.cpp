class Solution {
public:
    int minEatingSpeed(vector<int>& piles, int h) {
        int k = 1;

        while(true){
            long long count = 0;
            for(int i =0; i<piles.size(); i++){
                count+= (piles[i]+k-1)/k;
            }
            if(count <=h){
                return k;
            }
            k++;
        }
    }
};
