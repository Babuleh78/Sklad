class Solution {
public:
    vector<int> productExceptSelf(vector<int>& nums) {
        vector<int> r = {1};
        vector<int> l = {1};
        vector<int> ans;
        for(int i = 1; i<nums.size(); i++){
            r.push_back(r[i-1]*nums[i-1]);
        }
        for(int i = nums.size()-2; i>=0; i--){
            int number = nums[i+1]*l[0];
            l.insert(l.begin(), number);
        }
        
        int size = r.size()-1;
        for(int i = 0; i <nums.size(); i++){
            ans.push_back(r[i]*l[i]);
        }
        return ans;
    }
};
