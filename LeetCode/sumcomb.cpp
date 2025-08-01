class Solution {
public:
    vector<vector<int>> combinationSum(vector<int>& candidates, int target) {
        vector<vector<int>> ans;
        vector<int> comb;
        helper(candidates, ans, comb, target, 0, 0);
        return ans;
    }

    void helper(vector<int>& candidates, vector<vector<int>>& ans, vector<int>comb, int target, int current, int idx){
        if(target == current){
            ans.push_back(comb);
            return;
        }

        if(current>target || idx >= candidates.size()){
            return;
        }

        comb.push_back(candidates[idx]);
        helper(candidates, ans, comb, target, current+candidates[idx], idx);
        comb.pop_back();
        helper(candidates, ans, comb, target, current, idx+1);
    }
};
