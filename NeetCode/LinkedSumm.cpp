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
    ListNode* addTwoNumbers(ListNode* l1, ListNode* l2) {
        int dop = 0;
        ListNode* ans = new ListNode();
         ListNode* Dummy = ans;
        while(l1!=nullptr && l2!=nullptr){
            int s = l1->val+l2->val;
            ans->val = s%10+dop;
            cout << ans->val;
            dop = s/10;
            l1 = l1->next;
            l2 = l2->next;
            if(l1!=nullptr && l2!=nullptr){
                ans->next= new ListNode();
                ans = ans->next;
            }
        }
        if(l1!=nullptr or l2!=nullptr){
            ans->next= new ListNode();
            ans = ans->next;
        }
        while(l1!=nullptr){
            
            int s = (l1->val+dop);
            ans->val = s%10;
            dop = s/10;
            l1 = l1->next;
            if(l1!=nullptr){
                ans->next= new ListNode();
                ans = ans->next;
            }
        }
         while(l2!=nullptr){
            int s = dop+l2->val;
            ans->val = s%10;
            dop = s/10;
            l2 = l2->next;
            if(l2!=nullptr){
                ans->next= new ListNode();
                ans = ans->next;
            }
        }
        if(dop!=0){
            ans->next = new ListNode();
            ans = ans->next;
            ans->val = dop;
        }
        return Dummy;
    }
};



class Solution {
public:
    ListNode* addTwoNumbers(ListNode* l1, ListNode* l2) {
        ListNode* dummyHead = new ListNode(0); // Создаем "пустую" голову для результирующего списка
        ListNode* current = dummyHead; // Указатель на текущий узел
        int carry = 0; // Перенос

        while (l1 != nullptr || l2 != nullptr || carry != 0) {
            int sum = carry; // Начинаем с переноса

            if (l1 != nullptr) {
                sum += l1->val; // Добавляем значение из первого списка
                l1 = l1->next; // Переходим к следующему узлу
            }

            if (l2 != nullptr) {
                sum += l2->val; // Добавляем значение из второго списка
                l2 = l2->next; // Переходим к следующему узлу
            }

            carry = sum / 10; // Вычисляем новый перенос
            current->next = new ListNode(sum % 10); // Создаем новый узел с остатком
            current = current->next; // Переходим к следующему узлу
        }

        return dummyHead->next; // Возвращаем следующий узел после "пустой" головы
    }
};
