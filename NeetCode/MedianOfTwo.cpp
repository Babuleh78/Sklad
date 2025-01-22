class Solution {
public:
    double findMedianSortedArrays(vector<int>& nums1, vector<int>& nums2) {
        vector<int> vec;
        int p1 = 0;
        int p2 = 0;
        while(p1<nums1.size() && p2<nums2.size()){
            if(nums1[p1]<nums2[p2]){
                vec.push_back(nums1[p1]);
                p1++;
            } else{
                vec.push_back(nums2[p2]);
                p2++;
            }
        }
        for(int i = p1; i<nums1.size();i++){
            vec.push_back(nums1[i]);
        }
        for(int i = p2; i<nums2.size();i++){
            vec.push_back(nums2[i]);
        }
        for(int i = 0; i<vec.size(); i++){
            cout << vec[i];
        }
        int n = vec.size();
        if(n%2 == 1){
            return vec[n/2];
        } else{
            return (vec[n/2]+vec[n/2 -1])/2.0;
        }
        return 0;
    }
};
