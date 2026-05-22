# Changelog 自动生成机制设计方案

## 背景与目标

为 stringplus 项目引入自动化 changelog 生成机制，使版本发布流程标准化。开发者只需执行一条命令，即可完成版本号推断、CHANGELOG 生成、版本提交和标签创建。

## 方案选择

采用 **commit-and-tag-version** 方案。

### 选择理由

- 项目已具备 Node.js 工具链（commitlint + husky + npm），引入成本最低
- 专为 Conventional Commits 设计，与现有提交规范无缝衔接
- 一个命令完成版本推断、CHANGELOG 生成、提交和打标签全流程
- 维护活跃，是 `standard-version` 的推荐继任者

## 依赖与配置

### 新增依赖

```json
"devDependencies": {
  "commit-and-tag-version": "^12.0.0"
}
```

### package.json 脚本

```json
{
  "scripts": {
    "release": "commit-and-tag-version"
  }
}
```

### 版本来源

以 `package.json` 中的 `version` 字段作为当前版本基准。`commit-and-tag-version` 将基于该值进行递增。

## 执行流程

开发者执行 `npm run release` 后，工具自动完成以下步骤：

1. 读取当前 `package.json` 中的 `version`
2. 扫描自上一次 git tag 以来的所有 commits
3. 根据 Conventional Commits 规则推断新版本号：
   - 存在 `BREAKING CHANGE` → major 递增
   - 存在 `feat` 类型 commit → minor 递增
   - 仅存在 `fix` 等其他类型 → patch 递增
4. 更新 `package.json` 中的 `version`
5. 按 Angular 预设格式生成或追加 `CHANGELOG.md`
6. 自动 `git add` 相关文件并提交一条格式为 `chore(release): X.Y.Z` 的 commit
7. 创建对应版本号的 git tag（如 `v1.1.0`）

### 预览模式

如需先预览变更而不实际执行，可运行：

```bash
npm run release -- --dry-run
```

## CHANGELOG 格式

采用 `commit-and-tag-version` 默认预设（Angular 约定），按以下类型分组：

| 分组 | 对应 commit type |
|------|-----------------|
| Features | feat |
| Bug Fixes | fix |
| Performance Improvements | perf |
| Documentation | docs |
| Styles | style |
| Code Refactoring | refactor |
| Tests | test |
| Build System / CI | build, ci |
| Chores | chore |

如有 `BREAKING CHANGE` 标记，将在 CHANGELOG 顶部独立展示为 **BREAKING CHANGES**。

## 约束与注意事项

1. **干净的工作目录**：执行前必须确保 `git status` 无未提交修改，否则工具将拒绝执行。
2. **与现有工具链兼容**：自动生成的 `chore(release)` commit 和 tag 会经过现有 husky + commitlint hook 校验，格式符合规范，可正常通过。
3. **CMake 版本同步**：当前 `CMakeLists.txt` 未设置项目版本号，暂不需要额外同步逻辑。若未来需要，可扩展 `release` 脚本使用生命周期 hook 实现自动同步。
4. **首次发布**：若项目尚无 git tag，工具将以 `v0.0.0` 为基准，根据所有历史 commits 推断首个版本号。

## 相关文件变更

- `package.json`：新增依赖和 `release` 脚本
- `CHANGELOG.md`：首次运行时自动生成

## 排除的方案

### commitizen + cz-conventional-changelog

项目已通过 commitlint + husky 约束提交规范，不需要交互式提交辅助；`cz bump` 功能相对实验性，CHANGELOG 格式不如 commit-and-tag-version 完善。

### 自定义 npm script 脚本

需要额外编写和维护脚本代码，违背"引入现成机制"的初衷，且 `conventional-changelog-cli` 的组合配置更为复杂。
