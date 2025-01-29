/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     ListNode *next;
 *     ListNode() : val(0), next(nullptr) {}
 *     ListNode(int x) : val(x), next(nullptr) {}
 *     ListNode(int x, ListNode *next) : val(x), next(next) {}
 * };
 */

class Solution {
public:
    ListNode* removeNthFromEnd(ListNode* head, int n) {
        ListNode* Dummy = new ListNode();
        Dummy->next = head;
        if(head->next == nullptr){
            return nullptr;
        }
        ListNode* Replica = head;
        while(n!=0){
            Replica = Replica->next;
            n--;
        }
        if(Replica == nullptr){
            head = head->next;
            return head;
        }
        while(Replica->next!=nullptr){
            Replica = Replica->next;
            head = head->next;
        }
        head->next = head->next->next;
        return Dummy->next;
    }
};
