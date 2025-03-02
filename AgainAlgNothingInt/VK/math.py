N = int(input())
array = list(map(int, input().split()))
zero_count = 0
for i in range(N):
    if(array[i]!=0):
        print(array[i], end = " ")
    else:
        zero_count+=1
for i in range(zero_count):
    print(0, end = " ")
