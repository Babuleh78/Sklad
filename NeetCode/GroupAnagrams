    class Solution {
    public:
        vector<vector<string>> groupAnagrams(vector<string>& strs) {
            vector<vector<string>> ans;
            unordered_map<string, int> map; //Строка - позиция в векторе+1
            
            for (int i = 0; i <strs.size(); i++){
                string word = strs[i];
                sort(word.begin(), word.end());
                
                if(map[word]==0){
                    vector<string> new_vec;
                    new_vec.push_back(strs[i]); 
                    ans.push_back(new_vec);
                    map[word] = ans.size();
                } else{
                    ans[map[word]-1].push_back(strs[i]);
                }
            }
            return ans;
        }
    };
