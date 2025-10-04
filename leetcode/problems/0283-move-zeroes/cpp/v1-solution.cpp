/*
 * LeetCode 283. Move Zeroes (Easy)
 * Link: https://leetcode.cn/problems/move-zeroes
 * Tags: Array, Two Pointers
 * Approach: TODO | Time: O(?) | Space: O(?)
 */

 /*
 给定一个数组 nums，编写一个函数将所有 0 移动到数组的末尾，同时保持非零元素的相对顺序。
请注意 ，必须在不复制数组的情况下原地对数组进行操作。
 */
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    // TODO: 实现你的解法
    void moveZeroes(vector<int>& nums) {
        int n = nums.size();
        int j = 0; // 指向下一个非零元素应该放置的位置
        for (int i = 0; i < n; ++i) {
            if (nums[i] != 0) {
                if (i != j) {
                    swap(nums[i], nums[j]);
                }
                ++j;
            }
        }
    }
};

// ===== 本地简单测试 =====
// g++ -std=c++17 -O2 -pipe -static -s -o main v1-solution.cpp && ./main
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    // TODO: 添加你的本地测试
    Solution sol;
    vector<int> nums = {0, 1, 0, 3, 12};
    sol.moveZeroes(nums);
    for (int num : nums) {
        cout << num << " ";
    }
    return 0;
}
