class Solution {
public:
    int search(vector<int>& nums, int target) {
        if(nums[0] == target){
            return 0;
        } else if(nums.size() == 1 && nums[0]!=target){
            return -1;
        }
        int left = 0;
        int right = nums.size()-1;
        while(left<=right){
            int mid = (left+right)/2;
            cout << mid << endl;
            if(nums[mid] == target){
                return mid;
            }
            if (nums[left] <= nums[mid]) { 
                if (nums[left] <= target && target < nums[mid]) {
                    right = mid - 1; 
                } else {
                    left = mid + 1; 
                }
            } else {
                if (nums[mid] < target && target <= nums[right]) {
                    left = mid + 1; 
                } else {
                    right = mid - 1; 
                }
            }
        }
        return -1;
    }
};
