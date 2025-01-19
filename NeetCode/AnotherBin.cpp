class Solution {
public:
    int findMin(vector<int> &nums) {
        if (nums.size() == 1) {
            return nums[0];
        }
        
        int left = 0;
        int right = nums.size() - 1;
        while (left < right) {
            int mid = (right + left) / 2;
            if (mid > 0 && nums[mid] < nums[mid - 1] && mid < nums.size() - 1 && nums[mid] < nums[mid + 1]) {
                return nums[mid];
            }
            if (nums[mid] > nums[right]) {
                left = mid + 1;
            } else {
                right = mid;
            }
        }

        return nums[left];
    }
};
