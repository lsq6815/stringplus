# Changelog 自动生成机制实现计划

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** 为 stringplus 项目配置 commit-and-tag-version，实现一条命令完成版本推断、CHANGELOG 生成、提交和打标签的自动化流程。

**Architecture:** 在现有 Node.js 工具链（commitlint + husky）基础上，引入 `commit-and-tag-version` 作为 devDependency，并通过 `package.json` 的 `scripts` 暴露 `npm run release` 命令。

**Tech Stack:** Node.js, npm, commit-and-tag-version, husky, commitlint

---

### Task 1: 安装 commit-and-tag-version 依赖

**Files:**
- Modify: `package.json`
- Modify: `package-lock.json`（自动生成）

- [ ] **Step 1: 安装依赖**

Run: `npm install --save-dev commit-and-tag-version`
Expected: 安装成功，无错误输出

- [ ] **Step 2: 验证 package.json**

确认 `package.json` 的 `devDependencies` 中新增了：
```json
"commit-and-tag-version": "^12.0.0"
```

- [ ] **Step 3: Commit**

```bash
git add package.json package-lock.json
git commit -m "chore: 安装 commit-and-tag-version 依赖"
```

---

### Task 2: 配置 release 脚本

**Files:**
- Modify: `package.json`

- [ ] **Step 1: 修改 package.json 添加 release 脚本**

在 `package.json` 的 `scripts` 中添加 `"release": "commit-and-tag-version"`：

```json
{
  "scripts": {
    "test": "echo \"Error: no test specified\" && exit 1",
    "prepare": "husky",
    "release": "commit-and-tag-version"
  }
}
```

- [ ] **Step 2: 验证 JSON 语法**

Run: `node -e "JSON.parse(require('fs').readFileSync('package.json', 'utf8'))"`
Expected: 无输出（表示 JSON 解析成功，无语法错误）

- [ ] **Step 3: Commit**

```bash
git add package.json
git commit -m "chore: 添加 release 脚本"
```

---

### Task 3: 验证工具安装与配置

**Files:**
- None（仅执行命令验证）

- [ ] **Step 1: 验证命令可识别**

Run: `npx commit-and-tag-version --help`
Expected: 输出帮助信息，包含 `--dry-run` 等选项

- [ ] **Step 2: 预览首次运行效果（不实际执行）**

Run: `npm run release -- --dry-run`
Expected: 工具输出预期的版本号推断、CHANGELOG 预览和 git 操作计划，但不实际修改文件或创建 tag

**注意：** 如果当前工作目录有未提交的修改，此命令会报错拒绝执行。需先确保 `git status` 干净。

---

### Task 4: 首次运行生成 CHANGELOG

**Files:**
- Create: `CHANGELOG.md`
- Modify: `package.json`（版本号更新）

- [ ] **Step 1: 确认工作目录干净**

Run: `git status`
Expected: "nothing to commit, working tree clean"

- [ ] **Step 2: 执行首次发布**

Run: `npm run release`
Expected: 
- 推断出版本号（如 `1.0.0`）
- 生成 `CHANGELOG.md`
- 提交 `chore(release): 1.0.0` 的 commit
- 创建 `v1.0.0` 的 tag

- [ ] **Step 3: 验证生成结果**

1. 检查 `package.json` 中的 `version` 是否已更新
2. 检查 `CHANGELOG.md` 是否已生成且内容正确（包含历史 commits 的分组）
3. 检查 git log 是否包含 `chore(release)` commit
4. 检查 `git tag` 是否包含新版本 tag

- [ ] **Step 4: Commit（工具已自动提交）**

工具已自动提交并打 tag，无需手动 commit。但需推送 tag 到远程（如需要）：
```bash
git push --follow-tags origin main
```

---

## Self-Review Checklist

### Spec Coverage

| 设计需求 | 对应任务 |
|---------|---------|
| 安装 commit-and-tag-version | Task 1 |
| 配置 release 脚本 | Task 2 |
| 验证工具安装 | Task 3 |
| 首次运行生成 CHANGELOG | Task 4 |
| 支持 --dry-run 预览 | Task 3 Step 2 |
| 版本号基于 package.json | Task 4 Step 2 |
| 自动生成 chore(release) commit | Task 4 Step 2 |
| 自动创建 git tag | Task 4 Step 2 |

### Placeholder Scan

- [x] 无 "TBD", "TODO"
- [x] 无 "implement later"
- [x] 每个步骤包含具体命令和期望输出
- [x] 代码片段完整

### Type Consistency

- [x] 工具名称一致：`commit-and-tag-version`
- [x] 脚本名称一致：`release`
- [x] 提交类型一致：`chore`
