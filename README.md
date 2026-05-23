# stringplus

小型 C 库，封装了 `string.h` 并提供更高级的辅助函数，外加动态字符串数组（`ccary`）。使用 [CMake](https://cmake.org) 构建，用于练习。

## 依赖

- [CMake](https://cmake.org) — 构建系统（必需）
- [Doxygen](https://www.doxygen.nl/) — 文档生成（可选）
- [Node.js](https://nodejs.org/) + npm — 用于 commitlint 和 husky（可选，开发提交检查）

安装 Node 开发依赖：

```bash
npm install
```

> **注意：** 如果不需要提交信息检查和文档生成，可以跳过 npm 依赖和 Doxygen 的安装。CMake 为必需工具，需另行安装。

## 构建

```bash
mkdir build && cd build
cmake ..
cmake --build .
```

生成 `libstringplus.a`（静态库）和 `app`（演示 / 测试运行器）。

## 文档生成

本项目使用 [Doxygen](https://www.doxygen.nl/) 从源码注释自动生成 API 文档。

```bash
cd build
cmake --build . --target docs
```

生成的 HTML 文档位于 `build/docs/html/index.html`，使用浏览器打开即可查看。

> **注意：** 需要事先安装 Doxygen。未安装时构建系统会自动跳过文档生成，不影响其他构建流程。

## API

### stringplus.h

| 函数 | 说明 |
|------|------|
| `indexof(str, ch)` | `ch` 在 `str` 中首次出现的索引 |
| `indexamong(str, charset)` | `charset` 中任意字符在 `str` 中首次出现的索引 |
| `split(str, delims)` | 按 `delims` 中任意分隔符拆分 `str`；返回 `pccary` |
| `join(strset, jch)` | 用 `jch` 连接 `strset` 中的字符串；返回分配的字符串 |

`NOP`（`-1`）用作"未找到"的哨兵值。

### ccary.h

动态 C 字符串数组（`char**` + `size_t`）。调用者使用后必须调用 `ccary_destroy()`。

| 函数 | 说明 |
|------|------|
| `ccary_init()` | 创建新的 `pccary` |
| `ccary_destroy(pcca)` | 释放所有内存 |
| `ccary_append(pcca, str)` | 追加字符串 |
| `ccary_size(pcca)` | 获取元素数量 |
| `ccary_foreach(pcca, func)` | 遍历元素 |
| `ccary_display(pcca)` | 打印内容 |
| `ccary_clean(pcca)` | 清空元素，保留容器 |

> **注意：** `ccary_foreach()` 的回调中调用 `free()` 会导致内存泄漏。

## Conventional Commits

本项目使用 [Conventional Commits](https://www.conventionalcommits.org/) 规范。提交信息必须符合以下格式：

```
<type>(<scope>): <description>
```

**允许的 type：**

| Type | 说明 |
|------|------|
| `feat` | 新功能 |
| `fix` | Bug 修复 |
| `docs` | 文档更新 |
| `style` | 代码格式（不影响逻辑） |
| `refactor` | 代码重构 |
| `perf` | 性能优化 |
| `test` | 测试相关 |
| `chore` | 构建/工具/依赖更新 |
| `ci` | CI 配置 |
| `build` | 构建系统 |
| `revert` | 回滚提交 |

**示例：**

```bash
feat: 添加 indexof() 辅助函数
fix(ccary): 处理 append 中的 NULL 指针
docs: 更新 README 中的 API 表格
```

提交时会自动检查格式，不符合规范的提交将被拒绝。

## 版本发布

本项目使用 [commit-and-tag-version](https://github.com/absolute-version/commit-and-tag-version) 自动管理版本。基于 Conventional Commits 提交历史，自动推断版本号（patch/minor/major），并同步更新 `package.json`、`CHANGELOG.md` 和 git tag。

```bash
# 自动推断版本级别
npm run release

# 手动指定版本级别
npm run release -- --release-as minor
npm run release -- --release-as major
npm run release -- --release-as 1.2.3
```

发布后将自动创建版本提交和 git tag，使用以下命令推送：

```bash
git push --follow-tags origin main
```

## 许可证

MIT
