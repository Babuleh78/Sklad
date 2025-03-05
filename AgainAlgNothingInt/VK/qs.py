N = int(input())  
arr = list(map(int, input().split()))  

def quick_sort(arr):
    if len(arr) <= 1:
        return arr
    else:
        k = arr[len(arr) // 2] 
        left = [x for x in arr if x < k]  
        middle = [x for x in arr if x == k]  
        right = [x for x in arr if x > k]  
        return quick_sort(left) + middle + quick_sort(right)  


print(*quick_sort(arr))
