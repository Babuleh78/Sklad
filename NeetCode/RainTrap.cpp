class Solution {
public:
    int trap(vector<int>& h) {
        int l = 0;
        int res = 0;
        int r = h.size()-1;
        int leftM = h[l];
        int rightM = h[r];
        while(l<r){
            if (leftM < rightM) {
                l++;
                leftM = max(leftM, h[l]);
                res += leftM - h[l];
            } else {
                r--;
                rightM = max(rightM, h[r]);
                res += rightM - h[r];
            }
        }
        return res;
        
    }
};
