# 从注释生成文档功能设计

日期：2026-05-21

## 背景

stringplus 项目已使用 Doxygen 风格的注释（`/** ... */`、`@brief`、`@param`、`@return`）对 API 进行了标注，但尚未配置文档生成工具。本方案旨在引入自动化的文档生成流程，将源码注释转换为可浏览的 HTML 文档。

## 目标

- 从现有 Doxygen 风格注释自动生成 HTML API 文档
- 集成到 CMake 构建流程中，构建时自动生成
- 文档输出跟随构建目录（`build/docs/html`），`make clean` 时自动清理
- 无 Doxygen 时构建不受影响

## 方案

### 文件变更

1. **新增** `docs/Doxyfile.in` — Doxygen 配置模板
2. **修改** `CMakeLists.txt` — 添加文档生成逻辑
3. **修改** `AGENTS.md` — 同步添加注释风格约定

### Doxyfile.in 关键配置

```ini
PROJECT_NAME      = "stringplus"
INPUT             = @CMAKE_SOURCE_DIR@/src \
                    @CMAKE_SOURCE_DIR@/include \
                    @CMAKE_SOURCE_DIR@/README.md
OUTPUT_DIRECTORY  = @CMAKE_BINARY_DIR@/docs
GENERATE_HTML     = YES
HTML_OUTPUT       = html
GENERATE_LATEX    = NO
EXTRACT_ALL       = YES
USE_MDFILE_AS_MAINPAGE = @CMAKE_SOURCE_DIR@/README.md
```

### CMakeLists.txt 新增内容

```cmake
find_package(Doxygen)
if(DOXYGEN_FOUND)
    configure_file(
        ${CMAKE_SOURCE_DIR}/docs/Doxyfile.in
        ${CMAKE_BINARY_DIR}/Doxyfile
        @ONLY
    )
    add_custom_target(docs ALL
        COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_BINARY_DIR}/Doxyfile
        COMMENT "Generating API documentation with Doxygen"
    )
endif()
```

### 构建流程

运行 `cmake .. && make` 时：

1. CMake 配置阶段：`configure_file` 将 `docs/Doxyfile.in` 中的 `@CMAKE_SOURCE_DIR@` 和 `@CMAKE_BINARY_DIR@` 替换为实际路径，生成 `build/Doxyfile`
2. 构建阶段：`docs` 目标自动执行 `doxygen build/Doxyfile`
3. Doxygen 解析 `src/`、`include/` 和 `README.md` 中的注释，生成 `build/docs/html/index.html`

### 无 Doxygen 时的行为

- `find_package(Doxygen)` 静默失败
- 不创建 `docs` 目标
- 其余构建（库和可执行文件）不受影响
- 不会报错中断构建

### AGENTS.md 同步更新

在 `AGENTS.md` 的“关键约定”章节添加注释风格规范：

```markdown
## 注释风格

- 所有公共 API 使用 [Doxygen](https://www.doxygen.nl/) 风格注释
- 格式要求：
  - 使用 `/** ... */` 块注释
  - 使用 `@brief` 描述函数功能
  - 使用 `@param` 描述参数
  - 使用 `@return` 描述返回值
- 示例：
  ```c
  /**
   * @brief 函数简要说明
   *
   * @param str : 参数说明
   * @return 返回值说明
   */
  size_t example(const char *str);
  ```
```

## 依赖

- Doxygen（可选，仅在生成文档时需要）
- CMake 3.10.0+（已有）

## 验证方式

构建完成后，检查 `build/docs/html/index.html` 是否存在且内容正确。

## 风险与缓解

| 风险 | 缓解措施 |
|------|---------|
| 用户未安装 Doxygen | `find_package` 静默失败，构建继续 |
| 输出路径冲突 | 使用 `CMAKE_BINARY_DIR` 确保与构建目录一致 |
| 跨平台路径问题 | 使用 CMake 变量，避免硬编码路径 |
