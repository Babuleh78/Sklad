N = int(input())  
arr = list(map(int, input().split()))  
el = int(input())
def func(N, arr, el):
    left = 0
    right = N-1
    while(left<right):
        mid = (left+right)//2
        
        if(arr[mid] == el):
            return mid
        if(arr[mid]>el):
            right = mid
        else:
            left = mid
        if(right-left == 1):
            return right

print(func(N, arr, el))
