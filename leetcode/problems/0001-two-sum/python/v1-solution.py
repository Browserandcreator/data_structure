"""
LeetCode 1. Two Sum [Easy]
Link: https://leetcode.cn/problems/two-sum (source: leetcode.cn)
Tags: Array, Hash Table

Problem:
给定一个整数数组 `nums`&nbsp;和一个整数目标值 `target`，请你在该数组中找出 和为目标值 `target`&nbsp; 的那&nbsp;两个&nbsp;整数，并返回它们的数组下标。

你可以假设每种输入只会对应一个答案，并且你不能使用两次相同的元素。

你可以按任意顺序返回答案。

&nbsp;

示例 1：

输入：nums = [2,7,11,15], target = 9
输出：[0,1]
解释：因为 nums[0] + nums[1] == 9 ，返回 [0, 1] 。

示例 2：

输入：nums = [3,2,4], target = 6
输出：[1,2]

示例 3：

输入：nums = [3,3], target = 6
输出：[0,1]

&nbsp;

提示：

	- `2 &lt;= nums.length &lt;= 104`
	- `-109 &lt;= nums[i] &lt;= 109`
	- `-109 &lt;= target &lt;= 109`
	- 只会存在一个有效答案

&nbsp;

进阶：你可以想出一个时间复杂度小于 `O(n2)` 的算法吗？

Approach: TODO
Time: O(?), Space: O(?)
"""
class Solution(object):
    def twoSum(self, nums, target):
        """
        :type nums: List[int]
        :type target: int
        :rtype: List[int]
        """
        