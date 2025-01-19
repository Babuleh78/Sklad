class Solution {
public:
    int maxArea(vector<int>& h) {
        int left = 0;
        int right = h.size()-1;
        int V = 0;
        while(left<right){
            int res = min(h[left], h[right])*(right-left);
            V = max(res, V);
            if(h[left]<h[right]){
                left++;
            } else{
                right --;
            }
        }
        return V;
    }
};
