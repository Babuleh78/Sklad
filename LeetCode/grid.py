class Solution(object):
    def helper(self, mas, n, x, min_val):
        k = 0
        for i in range(len(mas)):
            if(mas[i] == n):
                continue
            elif(abs(mas[i] - n)%x !=0):
                return min_val, min_val
            else:
                if(min_val<=k):
                    return min_val, min_val
                k += abs(mas[i]-n)/x
        return k, min(k, min_val)
    
    def minOperations(self, grid, x):
        dict= {}
        mas = []
        for i in range(len(grid)):
            for j in range(len(grid[0])):
                mas.append(grid[i][j])
        sorted(mas)

        min_el = 10000000000000000000
        min_val = min_el
        for i in range(len(mas)):
            if(mas[i] not in dict):
                min_el, min_val = self.helper(mas, mas[i], x, min_val)
                dict[mas[i]] = min_el
        if(min_val == min_el and min_el == 1000000):
            return -1
        return min_val


grid =[[980,476,644,56],[644,140,812,308],[812,812,896,560],[728,476,56,812]]
x = 84
sol = Solution()
print(sol.minOperations(grid, x))