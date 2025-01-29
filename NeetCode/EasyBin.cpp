/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode() : val(0), left(nullptr), right(nullptr) {}
 *     TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
 *     TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
 * };
 */

class Solution {
public:
    TreeNode* invertTree(TreeNode* root) {
        
        Helper(root);
        return root;

    }

    void Helper(TreeNode* node){
        if(node == nullptr) return;
        TreeNode* cop_l = node->left;
        node->left = node->right;
        node->right = cop_l;
        Helper(node->left);
        Helper(node->right);
    }
};

class Solution {
public:
    int maxDepth(TreeNode* root) {
        if(!root) return 0;
        return Helper(root, 0);
    }
    int Helper(TreeNode* node, int n){
        if(node == nullptr) return 0;
        
        int l = Helper(node->left, n);
        int r = Helper(node->right, n);
        return max(l, r)+1;
    }
};

class Solution {
public:
    int diameterOfBinaryTree(TreeNode* root) {
        int res = 0;
        helper(root, res);
        return res;
    }

    int helper(TreeNode* node, int& res){
        if(!node) return 0;
        int left = helper(node->left, res);
        int right = helper(node->right, res);
        res = max(res, left+right);
        return 1+max(left, right);
    }
};

