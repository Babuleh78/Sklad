class Solution:
    def rob(self, nums: list[int]) -> int:
        if(len(nums)== 1):
            return nums[0]
        money = []
        money.append(nums[0])
        money.append(max(nums[0], nums[1]))
        for i in range(2, len(nums)):
            money.append(max(money[i-1], nums[i]+money[i-2]))
        print(money)
        return money[-1]
nums = nums = [2,7,9,3,1]
sol = Solution
print(sol.rob(sol, nums))

        