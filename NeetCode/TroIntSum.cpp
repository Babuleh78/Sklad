class Solution {
public:
    vector<int> twoSum(vector<int>& nums, int target) {
        int left = 0;
        int right = 1;
        while(left<nums.size()-1 && right<nums.size()){
            if(nums[left]+nums[right] == target){
                return {left+1, right+1};
            } else if(nums[left]+nums[right]>target){
                left++;
                right = left+1;
            } else{
                right++;
            }
            if(right == nums.size()){
                left++;
                right = left+1;
            }
        }
        cout << left << right;
        return {1, 0};
    }
};
//Да бля, я то же самое делаю
