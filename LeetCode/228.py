class Solution:
    def summaryRanges(self, nums: list[int]) -> list[str]:
        if not nums:
            return []
        s = []
        first = nums[0]
        second = first

        for i in range(1, len(nums)):
            if nums[i] == nums[i-1] + 1:
                second = nums[i]
            else:
                if first == second:
                    s.append(str(first))
                else:
                    s.append(f"{first}->{second}")
                first = nums[i]
                second = first
        if first == second:
            s.append(str(first))
        else:
            s.append(f"{first}->{second}")

        return s

s = Solution().summaryRanges([0, 2, 3, 4, 6, 8, 9])
print(s)  
