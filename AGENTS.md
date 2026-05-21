# AGENTS.md

## 项目概述

小型 C 库，封装了 `string.h` 并提供更高级的辅助函数，外加动态字符串数组（`ccary`）。使用 CMake 构建。

## 构建

```bash
mkdir -p build && cd build
cmake ..
make
```

- 生成静态库 `libstringplus.a` 和可执行文件 `app`。
- `app` 是演示程序 / 简易测试运行器（`src/main.c`）。

## 项目结构

| 路径 | 作用 |
|------|------|
| `src/stringplus.c`, `src/ccary.c` | 库实现 |
| `src/main.c` | 演示 / 简易测试运行器 |
| `include/stringplus.h`, `include/ccary.h` | 公共 API 头文件 |

## 关键约定

- 源文件中的 include 风格：`#include "../include/stringplus.h"`（相对于 `src/` 的相对路径）。
- `ccary` 是动态字符串数组（`char**` + `size_t`）。调用者使用后必须调用 `ccary_destroy()`。
- `split()` 和 `join()` 返回堆分配的数据；调用者负责释放。
- `NOP` 宏（`-1`）用作"未找到"的哨兵值。

## Conventional Commits

项目采用 [Conventional Commits](https://www.conventionalcommits.org/) 规范，通过 commitlint + husky 在本地拦截不合规提交。

提交格式：`<type>(<scope>): <description>`

允许的 `type`：`feat`, `fix`, `docs`, `style`, `refactor`, `perf`, `test`, `chore`, `ci`, `build`, `revert`

约束：
- **提交信息（commit message）使用中文书写**
- **文档使用中文书写**

相关配置：
- `commitlint.config.js` — commitlint 规则
- `.husky/commit-msg` — Git hook 脚本
- `package.json` — husky + commitlint 依赖
