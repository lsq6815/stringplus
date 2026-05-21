# stringplus

小型 C 库，封装了 `string.h` 并提供更高级的辅助函数，外加动态字符串数组（`ccary`）。使用 [CMake](https://cmake.org) 构建，用于练习。

## 构建

```bash
mkdir build && cd build
cmake ..
make
```

生成 `libstringplus.a`（静态库）和 `app`（演示 / 测试运行器）。

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

## 许可证

MIT
