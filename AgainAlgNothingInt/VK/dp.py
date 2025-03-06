

N = int(input())  
arr = list(map(int, input().split())) 
k = int(input())
def func(N, arr, K):
 
    dp = [[float('-inf')] * (K + 1) for _ in range(N + 1)]

    for i in range(1, N + 1):
        dp[i][1] = arr[i - 1] 
    for k in range(2, K + 1):   
        for i in range(k, N + 1):  
            dp[i][k] = max(dp[i - 1][k], dp[i - 1][k - 1] * arr[i - 1])

    return dp[N][K]

print(func(N, arr, k))
