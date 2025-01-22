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
