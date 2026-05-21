# 引入 Linter 和测试的设计方案

## 日期

2026-05-21

## 背景

项目目前是一个小型 C 库（stringplus + ccary），使用 CMake 构建。现有 `src/main.c` 作为演示/简易测试运行器，但缺乏系统化的代码质量检查和自动化测试。本方案旨在引入 linter 和正式测试框架，并在 AGENTS.md 中建立约束规范。

## 目标

1. 在编译阶段引入严格的静态检查（编译器警告 + clang-tidy）
2. 引入 Unity 测试框架，建立系统化的单元测试
3. 在 AGENTS.md 中约束：每次完成代码修改后必须运行 linter 和测试

## Linter 设计

### 编译器警告

在 `CMakeLists.txt` 中为 `stringplus` 目标添加以下编译选项：

- `-Wall` — 启用绝大多数警告
- `-Wextra` — 启用额外警告
- `-Werror` — 将警告视为错误，强制修复
- `-pedantic` — 严格遵守 C 标准
- `-Wstrict-prototypes` — 确保函数原型完整
- `-Wmissing-prototypes` — 检测未声明原型的全局函数
- `-Wold-style-definition` — 拒绝旧式函数定义

仅在 GCC/Clang 环境下添加，避免 MSVC 兼容性问题。

### clang-tidy

创建 `.clang-tidy` 配置文件，启用以下检查：

- `bugprone-*` — 常见 bug 模式检测
- `clang-analyzer-*` — Clang 静态分析器检查
- `cppcoreguidelines-*` — C++ 核心准则（对 C 代码也有参考价值）
- `performance-*` — 性能相关检查
- `portability-*` — 可移植性检查

在 `CMakeLists.txt` 中配置 `CMAKE_C_CLANG_TIDY`，使其在编译时自动运行。同时提供 `lint` 自定义目标，便于单独运行 clang-tidy。

## 测试设计

### 框架选择：Unity

使用 CMake `FetchContent` 自动下载 Unity（v2.6.0），无需手动管理依赖。

### 测试目录结构

```
tests/
├── test_stringplus.c   # 测试 stringplus 的函数
└── test_ccary.c        # 测试 ccary 的函数
```

### 测试覆盖范围

**test_stringplus.c：**
- `indexof()` — 查找存在的字符、不存在的字符、空字符串
- `indexamong()` — 查找多个字符、无匹配、空字符集
- `split()` — 正常分割、连续分隔符、空字符串、单元素
- `join()` — 正常连接、单元素数组、空数组

**test_ccary.c：**
- `ccary_init()` / `ccary_destroy()` — 构造与销毁
- `ccary_append()` — 追加字符串、空字符串
- `ccary_size()` — 空数组、非空数组的大小
- `ccary_clean()` — 清理后重用
- `ccary_foreach()` — 遍历回调正确性
- `ccary_display()` — 输出验证（可选）

### CMake 集成

- 在 `CMakeLists.txt` 中 `enable_testing()`
- 添加 `tests/CMakeLists.txt` 子项目
- 使用 `add_test()` 注册每个测试可执行文件到 CTest

## AGENTS.md 约束

新增 "代码质量约束" 章节，规定：

- 每次修改源代码后，必须执行构建（`cmake --build .`），确保编译器无警告
- 每次修改源代码后，必须执行测试（`ctest`），确保所有测试通过
- 提交代码前，必须通过上述两项检查
- 在 CI/本地开发环境中，linter 和测试作为门禁（gate）

## 风险与缓解

| 风险 | 缓解措施 |
|------|----------|
| 编译器警告升级导致现有代码编译失败 | 先修复现有代码的警告，再启用 `-Werror` |
| clang-tidy 报告大量问题 | 分阶段启用检查，先解决高优先级问题 |
| Unity 引入额外编译时间 | 使用 FetchContent，仅在首次构建时下载 |

## 验收标准

1. `cmake --build .` 成功且无警告
2. `ctest` 运行所有测试且全部通过
3. `.clang-tidy` 配置生效（可通过 `clang-tidy` 命令验证）
4. AGENTS.md 包含新的代码质量约束章节
