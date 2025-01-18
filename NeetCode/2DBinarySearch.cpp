class Solution {
public:
    bool searchMatrix(vector<vector<int>>& matrix, int target) {
        int current_row = -1;
        for(int i = 0; i <matrix.size(); i++){
            if(matrix[i][0]<= target && matrix[i][matrix[i].size()-1]>=target){
                current_row = i;
                break;
            }
        }
        if(current_row == -1){
            return false;
        }
        vector<int> vec = matrix[current_row];
        int left = 0, right = vec.size();

        while(left<= right){
            int mid = (left+right)/2;
            if(vec[mid] == target){
                return true;
            }
            if(vec[mid]<target){
                left = mid+1;
            } else{
                right = mid-1;
            }
        }
        return false;
    }
};
