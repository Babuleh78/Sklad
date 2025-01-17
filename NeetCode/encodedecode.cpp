class Solution {
public:
    string encode(vector<string>& strs) {
        string s = "";
        for (const string& str : strs) {
            s += to_string(str.size()) + '#' + str;  
        }
        return s;
    }

    vector<string> decode(string s) {
        vector<string> vec;
        int ptr = 0;
        while (ptr < s.size()) {
            int delimiterPos = s.find('#', ptr);
            int length = stoi(s.substr(ptr, delimiterPos - ptr));
            ptr = delimiterPos + 1; 
            string word = s.substr(ptr, length);
            vec.push_back(word);
            ptr += length; 
        }
        return vec;
    }
};
