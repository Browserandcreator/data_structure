\
#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
leet.py - LeetCode 题解仓库管理工具（增强版）

新增能力：
- 只输入 --id（题号），脚本自动从 LeetCode 获取 title/slug/difficulty/tags
- 支持 leetcode.com / leetcode.cn 切换（默认 com）
- 缓存题号→slug 索引到 .cache/slug_index.json，减少网络请求

依赖：
  pip install requests
"""

import argparse, os, json, csv, datetime, sys, re, time
from pathlib import Path

try:
    import requests
except Exception as e:
    requests = None

ROOT = Path(__file__).resolve().parents[1]
PROBLEMS = ROOT / "problems"
TEMPLATES = ROOT / "templates"
INDEX_CSV = ROOT / "INDEX.csv"
README_MD = ROOT / "README.md"
CACHE_DIR = ROOT / ".cache"
CACHE_DIR.mkdir(exist_ok=True)
SLUG_CACHE = CACHE_DIR / "slug_index.json"

DEFAULT_SITE = "com"   # 可选 "com" 或 "cn"

HEADERS = {
    "User-Agent": "leetcode-solutions-script/1.0 (+https://github.com/)"
}

def slugify(s: str) -> str:
    s = s.strip().lower()
    s = re.sub(r"[^a-z0-9]+", "-", s)
    s = re.sub(r"-+", "-", s).strip("-")
    return s

def ensure_dir(p: Path):
    p.mkdir(parents=True, exist_ok=True)

def write_file(path: Path, content: str):
    ensure_dir(path.parent)
    path.write_text(content, encoding="utf-8")

def load_template(name: str) -> str:
    return (TEMPLATES / name).read_text(encoding="utf-8")

# --------------------- 网络获取区 ---------------------

def _site_base(site: str) -> str:
    return "https://leetcode.com" if site == "com" else "https://leetcode.cn"

def fetch_slug_index(site: str) -> dict:
    """获取 题号->slug 的映射。优先缓存，其次请求 /api/problems/all/"""
    # 先读缓存
    if SLUG_CACHE.exists():
        try:
            data = json.loads(SLUG_CACHE.read_text(encoding="utf-8"))
            if "site" in data and data.get("site") == site and "map" in data:
                return data["map"]
        except Exception:
            pass

    if requests is None:
        print("[warn] 未安装 requests，无法联网获取 slug index。请 pip install requests 或手动提供 --title/--slug/--difficulty/--tags")
        return {}

    url = f"{_site_base(site)}/api/problems/all/"
    try:
        resp = requests.get(url, headers=HEADERS, timeout=15)
        resp.raise_for_status()
        obj = resp.json()
    except Exception as e:
        print(f"[warn] 获取 {url} 失败：{e}")
        return {}

    mapping = {}
    for q in obj.get("stat_status_pairs", []):
        # 注意：CN 站 question_id 可能是字符串，转换成 int
        try:
            qid = int(q.get("stat", {}).get("frontend_question_id"))
        except Exception:
            continue
        slug = q.get("stat", {}).get("question__title_slug", "")
        if qid and slug:
            mapping[qid] = slug

    # 写缓存
    SLUG_CACHE.write_text(json.dumps({"site": site, "map": mapping}, ensure_ascii=False), encoding="utf-8")
    return mapping

def fetch_question_detail_by_slug(slug: str, site: str) -> dict:
    """通过 GraphQL 获取题目详情"""
    if requests is None:
        return {}

    url = f"{_site_base(site)}/graphql/"
    payload = {
        "operationName": "questionData",
        "variables": {"titleSlug": slug},
        "query": """
        query questionData($titleSlug: String!) {
          question(titleSlug: $titleSlug) {
            questionId
            title
            titleSlug
            difficulty
            topicTags { name slug }
            translatedTitle
            translatedContent
          }
        }
        """
    }
    try:
        resp = requests.post(url, json=payload, headers=HEADERS, timeout=15)
        resp.raise_for_status()
        data = resp.json()
        q = data.get("data", {}).get("question", {}) or {}
        return q
    except Exception as e:
        print(f"[warn] GraphQL 获取题目详情失败：{e}")
        return {}

def auto_fill_meta_by_id(qid: int, site: str) -> dict:
    """只给题号，自动补全 meta：title/slug/difficulty/tags/link"""
    slug_map = fetch_slug_index(site)
    slug = slug_map.get(qid)
    if not slug:
        print(f"[warn] 没有在 slug 索引中找到题号 {qid}，将退化为手动输入。")
        return {}

    q = fetch_question_detail_by_slug(slug, site)
    if not q:
        print(f"[warn] 未能获取 {slug} 的详情，将退化为手动输入。")
        return {}

    # 有些站点 questionId 可能是字符串
    try:
        q_front_id = int(q.get("questionId"))
    except Exception:
        q_front_id = qid

    title = q.get("title") or ""
    difficulty = q.get("difficulty") or ""
    tags = [t.get("name") for t in (q.get("topicTags") or []) if t.get("name")]
    link = f"{_site_base(site)}/problems/{slug}"

    return {
        "id": q_front_id,
        "title": title,
        "slug": slug,
        "difficulty": difficulty,
        "tags": tags,
        "link": link,
    }

# --------------------- 业务逻辑 ---------------------

def cmd_new(args):
    # 优先：如果只提供了 id，就自动补齐信息
    id_provided = args.id is not None
    auto_meta = {}

    site = args.site or DEFAULT_SITE

    if id_provided and not (args.title and args.slug and args.difficulty):
        auto_meta = auto_fill_meta_by_id(args.id, site)

    # 如果自动获取失败，则使用手动/默认
    if auto_meta:
        id_ = int(auto_meta["id"])
        title = auto_meta["title"]
        slug = auto_meta["slug"]
        difficulty = auto_meta["difficulty"]
        tags = auto_meta["tags"]
        link = auto_meta["link"]
    else:
        # 手工模式（兼容旧参数）
        if args.id is None:
            print("[error] 未提供 --id，且无法自动获取。请提供 --id 或完整的 --title/--slug/--difficulty/--tags")
            sys.exit(1)
        id_ = int(args.id)
        title = args.title or f"Problem {id_}"
        slug = args.slug or slugify(title)
        difficulty = args.difficulty or "Easy"
        tags = [t.strip() for t in (args.tags or "").split(",") if t.strip()]
        link = args.link or f"{_site_base(site)}/problems/{slug}"

    langs = [l.strip().lower() for l in args.langs]

    # 目录与文件
    dir_name = f"{id_:04d}-{slug}"
    prob_dir = PROBLEMS / dir_name
    ensure_dir(prob_dir)

    # NOTES.md
    notes_tpl = load_template("NOTES.md.txt")
    notes_content = notes_tpl.format(
        id=id_,
        title=title,
        difficulty=difficulty,
        link=link,
        tags=", ".join(tags) if tags else "",
        time_complexity="O(?)",
        space_complexity="O(?)",
    )
    write_file(prob_dir / "NOTES.md", notes_content)

    # 代码模板
    for lang in langs:
        if lang == "cpp":
            code_tpl = load_template("cpp.txt")
            code = code_tpl.format(
                id=id_,
                title=title,
                difficulty=difficulty,
                link=link,
                tags=", ".join(tags),
                approach="TODO",
                time_complexity="O(?)",
                space_complexity="O(?)",
                filename=f"v1-solution.cpp",
            )
            path = prob_dir / "cpp" / "v1-solution.cpp"
            write_file(path, code)
        elif lang in ("py", "python"):
            code_tpl = load_template("python.txt")
            code = code_tpl.format(
                id=id_,
                title=title,
                difficulty=difficulty,
                link=link,
                tags=", ".join(tags),
                approach="TODO",
                time_complexity="O(?)",
                space_complexity="O(?)",
            )
            path = prob_dir / "python" / "v1-solution.py"
            write_file(path, code)
        elif lang == "java":
            java_code = f"""\
/*
 * LeetCode {id_}. {title} ({difficulty})
 * Link: {link}
 * Tags: {", ".join(tags)}
 * Approach: TODO | Time: O(?) | Space: O(?)
 */
class Solution {{
    // TODO
}}
"""
            path = prob_dir / "java" / "v1-Solution.java"
            write_file(path, java_code)
        else:
            print(f"[warn] 未识别语言: {lang}, 跳过。")

    # meta.json
    meta = {
        "id": id_,
        "title": title,
        "slug": slug,
        "difficulty": difficulty,
        "tags": tags,
        "link": link,
        "lastUpdated": datetime.date.today().isoformat(),
        "solutions": []
    }
    # 收集 solutions
    for lang in langs:
        if lang == "cpp":
            meta["solutions"].append({"lang": "cpp", "file": "cpp/v1-solution.cpp"})
        elif lang in ("py", "python"):
            meta["solutions"].append({"lang": "python", "file": "python/v1-solution.py"})
        elif lang == "java":
            meta["solutions"].append({"lang": "java", "file": "java/v1-Solution.java"})

    write_file(prob_dir / "meta.json", json.dumps(meta, ensure_ascii=False, indent=2))
    print(f"[ok] 新建：{prob_dir}")

def scan_index():
    rows = []
    if not PROBLEMS.exists(): return rows
    for d in sorted(PROBLEMS.iterdir()):
        if not d.is_dir(): continue
        meta_path = d / "meta.json"
        if not meta_path.exists():
            # 尝试从目录名推断
            m = re.match(r"(\d{4})-(.+)", d.name)
            if not m:
                print(f"[warn] 跳过无 meta 的目录: {d.name}")
                continue
            id_ = int(m.group(1))
            slug = m.group(2)
            title = slug.replace("-", " ").title()
            difficulty = ""
            tags = ""
            langs = ";".join([p.name for p in d.iterdir() if p.is_dir()])
            lastUpdated = ""
        else:
            meta = json.loads(meta_path.read_text(encoding="utf-8"))
            id_ = meta.get("id", "")
            slug = meta.get("slug", "")
            title = meta.get("title", "")
            difficulty = meta.get("difficulty", "")
            tags = ";".join(meta.get("tags", []))
            langs = ";".join(sorted(set(s["lang"] for s in meta.get("solutions", []))))
            lastUpdated = meta.get("lastUpdated", "")

        rows.append({
            "id": id_,
            "slug": slug,
            "title": title,
            "difficulty": difficulty,
            "tags": tags,
            "langs": langs,
            "lastUpdated": lastUpdated,
        })
    return rows

def cmd_index(args):
    rows = scan_index()
    with INDEX_CSV.open("w", newline="", encoding="utf-8") as f:
        w = csv.DictWriter(f, fieldnames=["id","slug","title","difficulty","tags","langs","lastUpdated"])
        w.writeheader()
        for r in rows:
            w.writerow(r)
    print(f"[ok] 已生成 {INDEX_CSV}（{len(rows)} 条）")

def cmd_readme(args):
    # Load index
    if not INDEX_CSV.exists():
        print("[info] 未找到 INDEX.csv，先执行 index")
        cmd_index(args)
    rows = []
    with INDEX_CSV.open("r", encoding="utf-8") as f:
        r = csv.DictReader(f)
        rows = list(r)

    lines = []
    lines.append("# 🚀 LeetCode Solutions\n")
    lines.append("自动生成的题目索引（由 `scripts/leet.py readme` 生成）。\n")
    lines.append("| ID | Title | Difficulty | Tags | Languages | Last Updated |")
    lines.append("|---:|:------|:-----------|:-----|:----------|:-------------|")

    def keyfunc(x):
        try:
            return int(x["id"])
        except Exception:
            return 10**9

    for row in sorted(rows, key=keyfunc):
        id_ = row["id"]
        slug = row["slug"]
        title = row["title"]
        difficulty = row["difficulty"]
        tags = row["tags"].replace(";", ", ")
        langs = row["langs"].replace(";", ", ")
        last = row["lastUpdated"]
        folder = f"problems/{int(id_):04d}-{slug}" if str(id_).isdigit() and slug else ""
        title_md = f"[{title}]({folder})" if folder else title
        lines.append(f"| {id_} | {title_md} | {difficulty} | {tags} | {langs} | {last} |")

    README_MD.write_text("\n".join(lines) + "\n", encoding="utf-8")
    print(f"[ok] 已生成 {README_MD}")


def cmd_quick(args):
    """
    一键创建题目目录（自动补全）→ 生成索引 → 生成 README
    用法：quick --id 121 --langs cpp python [--site com|cn]
          或：quick 121 cpp python
    """
    # 兼容位置参数：如果 id 没传但传了 positional_id
    qid = args.id if args.id is not None else args.positional_id
    if qid is None:
        print("[error] quick 需要提供题号，如: quick --id 121 或 quick 121")
        return
    # 构造 new 的参数对象（复用逻辑）
    class _A:
        pass
    a = _A()
    a.id = int(qid)
    a.title = None
    a.slug = None
    a.difficulty = None
    a.tags = None
    a.link = None
    a.langs = args.langs or (args.positional_langs if args.positional_langs else ["cpp"])
    a.site = args.site

    # 1) new
    print(f"[quick] new --id {a.id} --langs {' '.join(a.langs)} --site {a.site}")
    cmd_new(a)

    # 2) index
    print("[quick] index")
    class _B: pass
    b = _B()
    cmd_index(b)

    # 3) readme
    print("[quick] readme")
    class _C: pass
    c = _C()
    cmd_readme(c)


def main():
    parser = argparse.ArgumentParser(description="LeetCode 仓库管理工具（增强版）")
    sub = parser.add_subparsers(dest="cmd", required=True)

    # quick subcommand
    p_quick = sub.add_parser("quick", help="一键：new(自动补全) + index + readme")
    p_quick.add_argument("--id", type=int, help="题号（整数）")
    p_quick.add_argument("positional_id", nargs="?", type=int, help="题号（位置参数，可省略 --id）")
    p_quick.add_argument("--langs", nargs="+", help="语言列表，如：cpp python java")
    p_quick.add_argument("positional_langs", nargs="*", help="语言（位置参数）")
    p_quick.add_argument("--site", choices=["com","cn"], default="com", help="默认 com，可用 cn")
    p_quick.set_defaults(func=cmd_quick)

    p_new = sub.add_parser("new", help="新建题目目录和模板；支持仅提供 --id 自动补全题目信息")
    p_new.add_argument("--id", type=int, help="题号（整数）")
    p_new.add_argument("--title", help="英文标题（可省略，若提供则覆盖自动获取）")
    p_new.add_argument("--slug", help="英文短名（可省略）")
    p_new.add_argument("--difficulty", choices=["Easy","Medium","Hard"], help="可省略，自动获取")
    p_new.add_argument("--tags", help="逗号分隔标签，如: Array,Hash Table（可省略，自动获取）")
    p_new.add_argument("--link", help="题目链接（默认按 slug 拼）")
    p_new.add_argument("--langs", nargs="+", default=["cpp"], help="要生成的语言：cpp python java ...")
    p_new.add_argument("--site", choices=["com","cn"], default="com", help="使用 leetcode.com 还是 leetcode.cn（默认 com）")
    p_new.set_defaults(func=cmd_new)

    p_idx = sub.add_parser("index", help="生成/更新 INDEX.csv")
    p_idx.set_defaults(func=cmd_index)

    p_readme = sub.add_parser("readme", help="根据 INDEX.csv 生成 README.md")
    p_readme.set_defaults(func=cmd_readme)

    args = parser.parse_args()
    args.func(args)

if __name__ == "__main__":
    main()
