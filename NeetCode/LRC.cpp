class Solution {
public:
    int characterReplacement(string s, int k) {
        map<char, int> M;
        int l = 0;
        int potl = 0, maxk = 0;
        for(int i = 0; i<s.size(); i++){
            M[s[i]]++;
            maxk = max(maxk, M[s[i]]);
            while((i-potl+1) - maxk >k){
                M[s[potl]]--;
                potl++;
            }
            l = max(l, i -potl+1);
        }

        return l;
    }
};
