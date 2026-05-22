# AGENTS.md

## 项目概述

小型 C 库，封装 `string.h` 并提供更高级的辅助函数，外加动态字符串数组（`ccary`）。使用 CMake 构建。

## 依赖

- CMake（必需）
- Node.js + npm（可选，用于 commitlint + husky）
- Doxygen（可选，用于文档生成）

安装 Node 依赖（如需要提交检查）：

```bash
npm install
```

## 构建与测试

```bash
cmake -S . -B build -G "MinGW Makefiles" -DCMAKE_C_COMPILER=gcc
cmake --build build
```

生成静态库 `libstringplus.a` 和可执行文件 `app`（演示 / 简易测试运行器）。

**运行测试：**

```bash
ctest --output-on-failure        # 运行所有测试
cd build && ./tests/test_stringplus   # 运行单个测试可执行文件
cd build && ./tests/test_ccary
```

> **注意：** 测试不使用 Unity 等框架，而是使用自定义的 `tests/test_utils.h` 宏（`TEST_ASSERT`, `RUN_TEST` 等）。

**生成文档：**

```bash
cmake --build . --target docs
```

生成的 HTML 位于 `build/docs/html/index.html`。

**手动运行 linter：**

```bash
cmake --build . --target lint
```

（需要安装 clang-tidy；未安装时构建会自动跳过。）

## 项目结构

| 路径 | 作用 |
|------|------|
| `src/stringplus.c`, `src/ccary.c` | 库实现 |
| `src/main.c` | 演示 / 简易测试运行器 |
| `include/stringplus.h`, `include/ccary.h` | 公共 API 头文件 |
| `tests/` | 单元测试（自定义测试框架） |

## 关键约定

- 源文件 include 风格：`#include "../include/stringplus.h"`（相对于 `src/` 的相对路径）。
- `ccary` 是动态字符串数组（`char**` + `size_t`）。调用者使用后必须调用 `ccary_destroy()`。
- `split()` 和 `join()` 返回堆分配的数据；调用者负责释放。
- `NOP` 宏（`((size_t)-1)`）用作"未找到"的哨兵值。
- `ccary_foreach()` 的回调中调用 `free()` 会导致内存泄漏。

## 注释风格

所有公共 API 使用 Doxygen 风格注释：

```c
/**
 * @brief 函数功能简述
 *
 * @param name : 参数说明
 * @return 返回值说明
 */
```

## 提交规范

采用 Conventional Commits，格式：`<type>(<scope>): <description>`

允许的 `type`：`feat`, `fix`, `docs`, `style`, `refactor`, `perf`, `test`, `chore`, `ci`, `build`, `revert`

约束：
- **提交信息使用中文书写**
- **文档使用中文书写**

相关配置：
- `commitlint.config.js` — commitlint 规则
- `.husky/commit-msg` — Git hook 脚本

## 构建约束

- CMake 生成目录**必须**使用 `build/`，不得使用其他目录。

## 代码质量约束

- 编译器警告全部视为错误（`-Werror`）。
- 每次修改源代码后，**必须**执行构建（`cmake --build .`），确保无警告。
- 每次修改源代码后，**必须**执行测试（`ctest --output-on-failure`），确保全部通过。
- 提交代码前，**必须**通过上述两项检查。
