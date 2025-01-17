class Solution {
public:
    vector<int> topKFrequent(vector<int>& nums, int k) {
        vector<int> ans;
        unordered_map<int, int> map;
        for(int i = 0; i<nums.size();i++){
            map[nums[i]]++;
        }
        vector<pair<int, int>> vec;
        for (const auto& p : map){
            vec.push_back({p.second, p.first});
        }
        sort(vec.rbegin(), vec.rend());
        for(int i = 0; i<k; i++){
            ans.push_back(vec[i].second);
        }
        return ans;
    }
};
