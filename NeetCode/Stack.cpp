class Solution {
public:
    bool isValid(string s) {
        stack<char> MyStack;
        for(int i = 0; i<s.size(); i++){
            if(MyStack.empty()){
                MyStack.push(s[i]);
            } else if(s[i] == ')'){
                if(MyStack.top() == '('){
                    MyStack.pop();
                } else{
                    return false;
                }
            } else if(s[i] == ']'){
                if(MyStack.top() == '['){
                    MyStack.pop();
                } else{
                    return false;
                }
            } else if(s[i] == '}'){
                if(MyStack.top() == '{'){
                    MyStack.pop();
                } else{
                    return false;
                }
            } else{
                MyStack.push(s[i]);
            }
        }
        return MyStack.empty();
    }
};
