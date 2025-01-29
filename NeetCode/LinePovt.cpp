class Solution {
public:
    bool checkInclusion(string s1, string s2) {
        if(s1.size() > s1.size()) return false;
        unordered_map<char, int> M1;
        for( char c: s1){
            M1[c]++;
        }
        int nado = M1.size();
        for(int i = 0; i<s2.size();i++){
            unordered_map<char, int> M2;
            int curr = 0;
            for(int j = i; j<s2.size(); j++){
                M2[s2[j]]++;
                if(M2[s2[j]] > M1[s2[j]]){
                    break;
                }
                if(M2[s2[j]] == M1[s2[j]]){
                    curr++;
                }
            }
            if(curr == nado){
                return true;
            }
        }
        return false;
    }
    
};
