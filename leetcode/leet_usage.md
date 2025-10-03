# 📖 leet.py 使用说明

这是一个用于管理 LeetCode 题解仓库的脚本，支持自动从 LeetCode 获取题目信息、生成代码模板、维护索引和 README。

---

## 🚀 命令一览

### 新建题目目录
```bash
python scripts/leet.py new --id 121 --langs cpp python
```
功能：
- 自动从 LeetCode 获取标题、slug、难度、标签  
- 生成 `problems/0121-best-time-to-buy-and-sell-stock/`  
- 包含：`cpp/v1-solution.cpp`、`python/v1-solution.py`、`NOTES.md`、`meta.json`

可选参数：
- `--site com|cn` 选择数据源（默认 com，可用 cn）  
- `--title/--slug/--difficulty/--tags` 手动覆盖自动结果  
- `--langs` 支持 `cpp` / `python` / `java`  

---

### 一键命令（推荐）
```bash
# 最简用法（位置参数：题号 + 语言）
python scripts/leet.py quick 121 cpp python

# 国内环境（使用 leetcode.cn）
python scripts/leet.py quick 121 cpp --site cn

# 只写 C++ 解法
python scripts/leet.py quick 146 cpp
```
等价于依次执行：
1. `new --id ...`  
2. `index`  
3. `readme`  

---

### 生成/更新索引
```bash
python scripts/leet.py index
```
功能：
- 扫描 `problems/` 目录  
- 更新 `INDEX.csv`，记录所有题目的 id、标题、难度、标签、语言、更新时间  

---

### 生成 README
```bash
python scripts/leet.py readme
```
功能：
- 读取 `INDEX.csv`  
- 自动生成/更新根目录 `README.md`，展示题目总览表  

---

## 📦 推荐工作流

1. 新建题目目录（推荐用一键命令）：  
   ```bash
   python scripts/leet.py quick 121 cpp python
   ```

2. 写解法：  
   - 编辑 `problems/.../cpp/v1-solution.cpp`  
   - 编辑 `problems/.../python/v1-solution.py`  

   写思路：  
   - 编辑 `problems/.../NOTES.md`  

3. 更新索引与 README：  
   ```bash
   python scripts/leet.py index
   python scripts/leet.py readme
   ```

4. 提交到 GitHub：  
   ```bash
   git add .
   git commit -m "feat(0121-best-time-to-buy-and-sell-stock): add cpp/python v1 solution"
   git push
   ```

---

## 📊 命令执行效果图（Mermaid）

```mermaid
flowchart TD
    A[quick/new] --> B[生成题目目录\n(meta.json, NOTES.md, 代码模板)]
    B --> C[index\n生成 INDEX.csv]
    C --> D[readme\n更新 README.md 总览]
    D --> E[git commit & push\n上传到 GitHub]
```
