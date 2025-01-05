class Solution {
public:
    bool isValidSudoku(vector<vector<char>>& board) {
        for(int i = 0; i<board[0].size();i++){//Строки
            vector<char> stroka = board[i];
            unordered_set<char> s;
            for(int j = 0; j <stroka.size(); j++){
                if(stroka[j]!='.'){
                    if(s.count(stroka[j])){
                        return false;
                    }
                    s.insert(stroka[j]);
                }
            }
        }

        for(int i = 0; i<board.size(); i++){//Столбцы
            unordered_set<char> s;
            for(int j = 0; j<board.size(); j++){
                if(board[j][i]!='.'){
                    if(s.count(board[j][i])){
                        return false;
                    }
                    s.insert(board[j][i]);
                }
            }
            
            
        }

        for(int sq = 0; sq<9; sq++){
            unordered_set<char> seen;
            for(int i =0; i<3; i++){
                for(int j = 0; j<3; j++){
                    int row = (sq/3)*3+i;
                    int col = (sq%3)*3 +j;
                    if(board[row][col] == '.'){
                        continue;
                    }
                    if(seen.count(board[row][col])){
                        return false;
                    }
                    seen.insert(board[row][col]);
                }
            }
        }




        return true;
    }
};
