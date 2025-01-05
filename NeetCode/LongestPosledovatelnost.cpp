class Solution {
public:
    int longestConsecutive(vector<int>& nums) {
        set<int> s;
        for(int i = 0; i<nums.size(); i++){
            s.insert(nums[i]);
        }
        int k = 0;
        int ans = 0;
        int last_el = 0;
        
        for(const int el : s){
            if(k == 0){
                k = 1;
                last_el = el;
                continue;
            }

            if(el-last_el == 1){
                k++;
            } else{
                ans = max(ans, k);
                k = 1;
            }
            last_el = el;
        }
        ans = max(ans, k);
        return ans;
    }
};
