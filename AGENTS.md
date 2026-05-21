# AGENTS.md

## 项目概述

小型 C 库，封装了 `string.h` 并提供更高级的辅助函数，外加动态字符串数组（`ccary`）。使用 CMake 构建。

## 依赖

- [CMake](https://cmake.org) — 构建系统
- [Node.js](https://nodejs.org/) + npm — 用于 commitlint 和 husky（开发提交检查）

安装 Node 依赖：

```bash
npm install
```

> **注意：** 如果不需要提交检查，可以跳过 `npm install`。CMake 和 Doxygen 为独立工具，需另行安装。

## 构建

```bash
mkdir -p build && cd build
cmake ..
cmake --build .
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
- `NOP` 宏（`((size_t)-1)`）用作"未找到"的哨兵值。
- `ccary_foreach()` 的回调中调用 `free()` 会导致内存泄漏。

## 注释风格

- 所有公共 API 使用 [Doxygen](https://www.doxygen.nl/) 风格注释，便于自动生成文档
- 格式要求：
  - 使用 `/** ... */` 块注释
  - 使用 `@brief` 描述函数功能
  - 使用 `@param` 描述参数（格式：`@param name : 说明`）
  - 使用 `@return` 描述返回值
- 示例：
  ```c
  /**
   * @brief index the first appearance of @ch in @str
   *
   * @param str : c-style string
   * @param ch  : char to been index
   * @return NOP if @str hasn't @ch, otherwise the index
   */
  size_t indexof(const char *str, int ch);
  ```

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

## 代码质量约束

- 每次修改源代码后，**必须**执行构建（`cmake --build .`），确保编译器无警告（所有警告视为错误）。
- 每次修改源代码后，**必须**执行测试（`ctest --output-on-failure`），确保所有测试通过。
- 提交代码前，**必须**通过上述两项检查。
- 如果安装了 clang-tidy，编译时会自动进行额外静态分析；也可通过 `cmake --build . --target lint` 手动运行。
