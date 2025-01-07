
class Solution {
public:
    int evalRPN(vector<string>& tokens) {
        stack<int> numbers;
        for(int i = 0; i<tokens.size(); i++){
            if(isNum(tokens[i])){
                cout << tokens[i];
                int n = stoi(tokens[i]);
                numbers.push(n);
            } else{
                int n2 = numbers.top();
                numbers.pop();
                int n1 = numbers.top();
                numbers.pop();
                char symb = tokens[i][0];
                int res = makeOper(n1, n2, symb);
                numbers.push(res);
                cout << numbers.top();
            }
        }
        return numbers.top();
    }


    bool isNum(string token){
        if (token.empty()) return false;
        int start = (token[0] == '-' && token.size() > 1) ? 1 : 0; 
        for (int i = start; i < token.size(); i++) {
            if (!isdigit(token[i])) {
                return false;
            }
        }
        return true;
    }

    int makeOper(int n1, int n2, char op){
        if(op == '+'){
            return n1+n2;
        }
        if (op == '-'){
            return n1-n2;
        }
        if(op == '*'){
            return n1*n2;
        }
        return n1/n2;
    }
};
