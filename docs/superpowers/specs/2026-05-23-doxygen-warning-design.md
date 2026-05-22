# Doxygen Warning 机制设计

日期：2026-05-23

## 背景

stringplus 项目已使用 Doxygen 风格注释对 API 进行标注，并配置了 `docs` target 用于生成 HTML 文档。但目前 `docs/Doxyfile.in` 未启用任何警告相关选项，导致文档注释中的错误（如参数名不匹配、unknown command）不会被检测，也不会导致构建失败。

当前已知的 Doxygen warnings 包括：
- `include/stringplus.h` 中 `@param ch` 与函数签名 `int chr` 不匹配
- 两个头文件描述中误用 `@参数名`（如 `@str`、`@pcca` 等）被解析为 unknown command

## 目标

建立严格的 Doxygen 文档质量检查机制，使文档错误与代码编译错误同等重要，与现有 `lint` target 统一集成。

## 改动内容

### 1. `docs/Doxyfile.in`

新增以下选项：

```
WARN_AS_ERROR          = YES
WARN_IF_UNDOCUMENTED   = YES
WARN_IF_DOC_ERROR      = YES
WARN_NO_PARAMDOC       = YES
```

### 2. `include/stringplus.h`

- 修复 `@param ch` → `@param chr`（与函数签名 `int chr` 匹配）
- 修复所有描述中误用的 `@str`、`@ch`、`@charset`、`@delims`、`@jch`、`@strset` 为普通文本

### 3. `include/ccary.h`

- 修复所有描述中误用的 `@pcca`、`@func`、`@str` 为普通文本

### 4. `CMakeLists.txt`

扩展 `lint` target，使其在运行 clang-tidy 之前（或之后）先执行 Doxygen 检查：

```cmake
if(DOXYGEN_FOUND AND CLANG_TIDY_EXE)
    add_custom_target(lint
        COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_BINARY_DIR}/Doxyfile
        COMMAND ${CLANG_TIDY_EXE} -p ${CMAKE_BINARY_DIR} ...
        COMMENT "Running Doxygen checks and clang-tidy..."
    )
elseif(DOXYGEN_FOUND)
    add_custom_target(lint
        COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_BINARY_DIR}/Doxyfile
        COMMENT "Running Doxygen checks..."
    )
elseif(CLANG_TIDY_EXE)
    # 保留现有 lint target
endif()
```

若只有一方可用，则 `lint` target 降级为仅运行可用工具。

## 风险与应对

- **风险**：`WARN_IF_UNDOCUMENTED = YES` 可能会发现更多目前未暴露的未文档化实体（如宏、枚举等）。
- **应对**：配置后先执行一次 `doxygen build/Doxyfile`，若存在额外未文档化实体，在修复阶段一并补充文档或按需调整选项。

## 与现有流程的集成

- 开发者运行 `cmake --build . --target lint` 时，文档检查自动执行。
- 与 `-Werror` 编译策略和 commitlint 提交检查形成完整质量闭环。
- `docs` target 仍正常生成文档，`WARN_AS_ERROR` 会让它在遇到任何文档错误时退出并返回非零码。
