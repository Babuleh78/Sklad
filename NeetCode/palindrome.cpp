class Solution {
public:
    bool isPalindrome(string s) {
        
        int ptrl = 0;
        int ptrr = s.size()-1;
        while(ptrr>ptrl){
             while(ptrr>ptrl && !IsAlphaNum(s[ptrl])){
                ptrl++;
            }
            while(ptrr>ptrl && !IsAlphaNum(s[ptrr])){
                ptrr--;
            }
        
            if(tolower(s[ptrr])!= tolower(s[ptrl])){
                return false;
            }
            ptrr--;
            ptrl++;
        }
        return true;
    }
    bool IsAlphaNum(char c) {
        return (c >= 'A' && c <= 'Z' || 
                c >= 'a' && c <= 'z' || 
                c >= '0' && c <= '9');
    }
};
