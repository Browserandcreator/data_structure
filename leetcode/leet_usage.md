# leet.py 使用说明

`leet.py` 是一个帮助管理本地 LeetCode 题目代码的脚本工具。

功能：

- **new / quick**：根据题号新建题目目录，自动补全元数据（标题、难度、标签、题干描述）；
- **自动注入题干描述和 OJ 提供的函数框架**（C++ / Python）；
- **remove**：删除误加的题目目录；
- **index**：重建 `INDEX.csv`，维护题目索引；
- **readme**：重建 `README.md`，自动生成题目表格。

目录结构（示例）：

```
problems/
  0135-candy/
    meta.json
    cpp/v1-solution.cpp
    python/v1-solution.py
INDEX.csv
README.md
```

---

## 常见命令

### 1. 新建题目

```bash
# 新建 135 号题 Candy，生成 C++ 和 Python 版本，使用英文站
python leet.py quick 135 cpp python

# 新建时优先使用中文站（题干为中文）
python leet.py quick 135 cpp python --site cn
```

生成的 `cpp/v1-solution.cpp` 文件示例：

```cpp
/*
 * LeetCode 135. Candy [Hard]
 * Link: https://leetcode.com/problems/candy
 * Tags: Greedy
 *
 * Problem:
 * n 个孩子站成一排，每个孩子有一个评分...
 *
 * Approach: TODO
 * Time: O(?), Space: O(?)
 */

class Solution {
public:
    int candy(vector<int>& ratings) {
        // 官方 OJ 框架已自动填充
    }
};
```

生成的 `python/v1-solution.py` 文件示例：

```python
"""
LeetCode 135. Candy [Hard]
Link: https://leetcode.com/problems/candy
Tags: Greedy

Problem:
n 个孩子站成一排，每个孩子有一个评分...

Approach: TODO
Time: O(?), Space: O(?)
"""

class Solution:
    def candy(self, ratings: List[int]) -> int:
        # 官方 OJ 框架已自动填充
        pass
```

---

### 2. 删除误加的题目

```bash
# 用题号删除
python leet.py remove --id 135

# 用目录名删除
python leet.py remove --dir 0135-candy
```

删除后会自动更新 `INDEX.csv` 和 `README.md`。

---

### 3. 重建索引与 README

```bash
# 重建索引文件 INDEX.csv
python leet.py index

# 重建 README.md（若 INDEX.csv 不存在会自动调用 index）
python leet.py readme
```

README 表格示例：

|   ID | Title                                        | Difficulty | Tags   | Path                  |
| ---: | -------------------------------------------- | :--------: | ------ | --------------------- |
|  135 | [Candy](https://leetcode.com/problems/candy) |    Hard    | Greedy | `problems/0135-candy` |

---

## 注意事项

1. **首次运行**需要网络访问 LeetCode，自动获取题目信息；
2. 若无网络或接口变化，可以手动修改 `meta.json`；
3. 自动生成的代码文件仅包含 **OJ 官方函数框架** + **题干描述**，需要你自己补充解答逻辑；
4. `--site cn` 优先取中文描述，`--site com` 优先取英文。

---

## 快速上手

```bash
# 新建题目并开始写解答
python leet.py quick 1 cpp python

# 删除错误题目
python leet.py remove --id 1

# 更新索引与 README
python leet.py index
python leet.py readme
```