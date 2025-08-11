class Solution {
public:
    void rotate(vector<vector<int>>& matrix) {
        int n = matrix.size();

        for(int x = 0; x<n; x++){
            for(int y = x+1; y<n; y++){
                int cop = matrix[x][y];
                matrix[x][y] = matrix[y][x];
                matrix[y][x] = cop;
            }
        }

        for (int x = 0; x < n; x++) {
            for (int y = 0; y < n / 2; y++) {
                int temp = matrix[x][y];
                matrix[x][y] = matrix[x][n - y - 1];
                matrix[x][n - y - 1] = temp;
            }
        }
    }
};
