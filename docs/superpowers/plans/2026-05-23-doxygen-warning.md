# Doxygen Warning 机制实现计划

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** 启用 Doxygen 警告作为错误，修复现有文档注释问题，并将文档检查集成到 lint target。

**架构：** 在 `Doxyfile.in` 中添加 `WARN_AS_ERROR` 等选项，修复 `include/*.h` 中的参数名不匹配和 unknown command 问题，扩展 `CMakeLists.txt` 的 `lint` target 以支持 Doxygen 检查。

**Tech Stack:** CMake, Doxygen, C

---

### Task 1: 修改 Doxyfile.in 启用警告选项

**Files:**
- Modify: `docs/Doxyfile.in`

- [ ] **Step 1: 在 Doxyfile.in 末尾追加警告配置**

```
# Warning configuration
WARN_AS_ERROR          = YES
WARN_IF_UNDOCUMENTED   = YES
WARN_IF_DOC_ERROR      = YES
WARN_NO_PARAMDOC       = YES
```

- [ ] **Step 2: 提交**

```bash
git add docs/Doxyfile.in
git commit -m "build(docs): enable Doxygen warnings as errors"
```

---

### Task 2: 修复 stringplus.h 中的文档注释

**Files:**
- Modify: `include/stringplus.h`

- [ ] **Step 1: 修复 indexof 函数的参数名和描述文本**

将：
```c
/**
 * @brief 查找 @ch 在 @str 中第一次出现的位置
 *
 * @param str : C 风格字符串
 * @param ch  : 要查找的字符
 * @return 如果 @str 中没有 @ch 返回 NOP，否则返回下标
 */
size_t indexof(const char *str, int chr);
```

改为：
```c
/**
 * @brief 查找 chr 在 str 中第一次出现的位置
 *
 * @param str : C 风格字符串
 * @param chr : 要查找的字符
 * @return 如果 str 中没有 chr 返回 NOP，否则返回下标
 */
size_t indexof(const char *str, int chr);
```

- [ ] **Step 2: 修复 indexamong 函数的参数名和描述文本**

将：
```c
/**
 * @brief 查找 @charset 中任意字符在 @str 中第一次出现的位置
 *
 * @param str     : C 风格字符串
 * @param charset : 要查找的字符集合
 * @return 如果 @str 中没有任何匹配字符返回 NOP，否则返回下标
 */
```

改为：
```c
/**
 * @brief 查找 charset 中任意字符在 str 中第一次出现的位置
 *
 * @param str     : C 风格字符串
 * @param charset : 要查找的字符集合
 * @return 如果 str 中没有任何匹配字符返回 NOP，否则返回下标
 */
```

- [ ] **Step 3: 修复 split 函数的参数名和描述文本**

将：
```c
/**
 * @brief 使用 @delims 中的分隔符分割 @str
 *
 * @param str    : 待分割的字符串
 * @param delims : 分隔符集合
 * @return 包含分割结果的 ccary
 */
```

改为：
```c
/**
 * @brief 使用 delims 中的分隔符分割 str
 *
 * @param str    : 待分割的字符串
 * @param delims : 分隔符集合
 * @return 包含分割结果的 ccary
 */
```

- [ ] **Step 4: 修复 join 函数的参数名和描述文本**

将：
```c
/**
 * @brief 使用 @jch 将 @strset 中的字符串连接成单个字符串
 *
 * @param strset : 字符串集合
 * @param jch    : 用于连接字符串的字符
 * @return 单个字符串
 */
```

改为：
```c
/**
 * @brief 使用 jch 将 strset 中的字符串连接成单个字符串
 *
 * @param strset : 字符串集合
 * @param jch    : 用于连接字符串的字符
 * @return 单个字符串
 */
```

- [ ] **Step 5: 提交**

```bash
git add include/stringplus.h
git commit -m "docs(stringplus.h): fix Doxygen parameter names and commands"
```

---

### Task 3: 修复 ccary.h 中的文档注释

**Files:**
- Modify: `include/ccary.h`

- [ ] **Step 1: 修复 ccary_destroy 函数的参数名和描述文本**

将：
```c
/**
 * @brief 销毁 @pcca 指向的 ccary，释放所有空间
 *
 * @param pcca : 指向 ccary 的指针
 */
```

改为：
```c
/**
 * @brief 销毁 pcca 指向的 ccary，释放所有空间
 *
 * @param pcca : 指向 ccary 的指针
 */
```

- [ ] **Step 2: 修复 ccary_foreach 函数的参数名和描述文本**

将：
```c
/**
 * @brief 遍历 @pcca 中的每个字符串并调用 @func
 * 注意：在 @func 中调用 free() 会导致内存泄漏！
 *
 * @param pcca : 指向 ccary 的指针
 * @param func : 处理字符串的函数
 */
```

改为：
```c
/**
 * @brief 遍历 pcca 中的每个字符串并调用 func
 * 注意：在 func 中调用 free() 会导致内存泄漏！
 *
 * @param pcca : 指向 ccary 的指针
 * @param func : 处理字符串的函数
 */
```

- [ ] **Step 3: 修复 ccary_clean 函数的参数名和描述文本**

将：
```c
/**
 * @brief 清理 @pcca 指向的所有空间，并将 size 置为 0
 *
 * @param pcca : 指向 ccary 的指针
 */
```

改为：
```c
/**
 * @brief 清理 pcca 指向的所有空间，并将 size 置为 0
 *
 * @param pcca : 指向 ccary 的指针
 */
```

- [ ] **Step 4: 修复 ccary_append 函数的参数名和描述文本**

将：
```c
/**
 * @brief 将 @str 追加到 @pcca
 *
 * @param pcca : 指向 ccary 的指针
 * @param str  : C 风格字符串
 */
```

改为：
```c
/**
 * @brief 将 str 追加到 pcca
 *
 * @param pcca : 指向 ccary 的指针
 * @param str  : C 风格字符串
 */
```

- [ ] **Step 5: 修复 ccary_display 函数的参数名和描述文本**

将：
```c
/**
 * @brief 显示 @pcca 的内容
 *
 * @param pcca : 指向 ccary 的指针
 */
```

改为：
```c
/**
 * @brief 显示 pcca 的内容
 *
 * @param pcca : 指向 ccary 的指针
 */
```

- [ ] **Step 6: 提交**

```bash
git add include/ccary.h
git commit -m "docs(ccary.h): fix Doxygen parameter names and commands"
```

---

### Task 4: 扩展 CMakeLists.txt 的 lint target

**Files:**
- Modify: `CMakeLists.txt`

- [ ] **Step 1: 找到现有 lint target 定义并重构**

将现有的：
```cmake
# Lint target (runs clang-tidy manually)
if(CLANG_TIDY_EXE)
    add_custom_target(lint
        COMMAND ${CLANG_TIDY_EXE}
            -p ${CMAKE_BINARY_DIR}
            ${CLANG_TIDY_EXTRA_ARGS}
            ${CMAKE_SOURCE_DIR}/src/*.c
            ${CMAKE_SOURCE_DIR}/include/*.h
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        COMMENT "Running clang-tidy..."
    )
endif()
```

改为：
```cmake
# Lint target (runs Doxygen and clang-tidy)
set(LINT_COMMANDS)
set(LINT_COMMENTS)

if(DOXYGEN_FOUND)
    list(APPEND LINT_COMMANDS
        COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_BINARY_DIR}/Doxyfile
    )
    list(APPEND LINT_COMMENTS "Doxygen")
endif()

if(CLANG_TIDY_EXE)
    list(APPEND LINT_COMMANDS
        COMMAND ${CLANG_TIDY_EXE}
            -p ${CMAKE_BINARY_DIR}
            ${CLANG_TIDY_EXTRA_ARGS}
            ${CMAKE_SOURCE_DIR}/src/*.c
            ${CMAKE_SOURCE_DIR}/include/*.h
    )
    list(APPEND LINT_COMMENTS "clang-tidy")
endif()

if(LINT_COMMANDS)
    string(JOIN " + " LINT_COMMENT_STRING ${LINT_COMMENTS})
    add_custom_target(lint
        ${LINT_COMMANDS}
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        COMMENT "Running ${LINT_COMMENT_STRING}..."
    )
endif()
```

- [ ] **Step 2: 提交**

```bash
git add CMakeLists.txt
git commit -m "build(cmake): integrate Doxygen into lint target"
```

---

### Task 5: 构建并验证

- [ ] **Step 1: 重新配置构建目录**

```bash
cd build && cmake ..
```

- [ ] **Step 2: 运行 lint target 验证 Doxygen 检查**

```bash
cmake --build . --target lint
```

Expected: 成功执行，无 Doxygen warnings（因为之前已修复）

- [ ] **Step 3: 运行完整测试套件**

```bash
ctest --output-on-failure
```

Expected: 所有测试通过

- [ ] **Step 4: 验证 docs target 仍可用**

```bash
cmake --build . --target docs
```

Expected: 文档生成成功，无 warnings

- [ ] **Step 5: 提交**

```bash
git commit --allow-empty -m "chore: verify Doxygen warning mechanism works"
```

---

## 自审检查

1. **Spec 覆盖：**
   - ✅ `Doxyfile.in` 中添加 `WARN_AS_ERROR` 等选项 → Task 1
   - ✅ 修复 `stringplus.h` 参数名不匹配和 unknown command → Task 2
   - ✅ 修复 `ccary.h` 参数名不匹配和 unknown command → Task 3
   - ✅ `lint` target 集成 Doxygen 检查 → Task 4
   - ✅ 构建和测试验证 → Task 5

2. **Placeholder 检查：**
   - ✅ 无 "TBD"、"TODO"、"implement later"
   - ✅ 所有步骤包含具体代码或命令
   - ✅ 无模糊描述

3. **类型一致性：**
   - ✅ 参数名与函数签名一致（`chr` 而非 `ch`）
   - ✅ CMake 变量名前后一致
