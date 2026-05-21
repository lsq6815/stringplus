# 引入 Linter 和测试 实现计划

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** 为 stringplus 项目引入编译器严格警告、clang-tidy 和 Unity 测试框架，并在 AGENTS.md 中建立代码质量约束。

**Architecture:** 通过 CMake 配置编译器警告选项和 clang-tidy 集成；使用 FetchContent 引入 Unity 测试框架；在 `tests/` 目录下编写单元测试并注册到 CTest；更新 AGENTS.md 添加质量门禁约束。

**Tech Stack:** CMake, GCC/Clang, clang-tidy, Unity (C testing framework)

---

## 文件结构

| 文件 | 操作 | 说明 |
|------|------|------|
| `CMakeLists.txt` | 修改 | 添加编译器警告、C 标准、clang-tidy、测试子目录 |
| `tests/CMakeLists.txt` | 创建 | Unity 和测试可执行文件的构建配置 |
| `tests/test_stringplus.c` | 创建 | stringplus 函数单元测试 |
| `tests/test_ccary.c` | 创建 | ccary 函数单元测试 |
| `.clang-tidy` | 创建 | clang-tidy 检查规则配置 |
| `src/main.c` | 修改 | 移除未使用的头文件包含 |
| `src/ccary.c` | 修改 | 移除未使用的头文件包含 |
| `AGENTS.md` | 修改 | 添加代码质量约束章节 |

---

### Task 1: 清理未使用的头文件包含

**Files:**
- Modify: `src/main.c:3-9`
- Modify: `src/ccary.c:3-9`

**说明：** 在启用 `-Wall -Werror` 之前，必须先修复现有代码中未使用头文件包含导致的警告。

- [ ] **Step 1: 移除 main.c 中未使用的头文件**

在 `src/main.c` 中，`<stdint.h>`、`<stddef.h>`、`<stdbool.h>`、`<ctype.h>`、`<time.h>`、`<math.h>` 均未被使用。

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/stringplus.h"
#include "../include/ccary.h"
```

- [ ] **Step 2: 移除 ccary.c 中未使用的头文件**

在 `src/ccary.c` 中，`<stdint.h>`、`<stddef.h>`、`<stdbool.h>`、`<ctype.h>`、`<time.h>`、`<math.h>` 均未被使用。

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/ccary.h"
```

- [ ] **Step 3: 构建验证**

Run: `cmake --build build`
Expected: 成功编译，无新增错误

- [ ] **Step 4: Commit**

```bash
git add src/main.c src/ccary.c
git commit -m "style: 移除未使用的头文件包含"
```

---

### Task 2: 在 CMakeLists.txt 中添加编译器警告和 C 标准

**Files:**
- Modify: `CMakeLists.txt:9-25`

- [ ] **Step 1: 设置 C 标准并添加编译器警告选项**

在 `CMakeLists.txt` 中，在 `project(stringplus)` 之后、目标定义之前添加：

```cmake
# Set C standard
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Compiler warnings
if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(
        -Wall
        -Wextra
        -Werror
        -pedantic
        -Wstrict-prototypes
        -Wmissing-prototypes
        -Wold-style-definition
        -Wshadow
        -Wpointer-arith
        -Wcast-qual
    )
endif()
```

- [ ] **Step 2: 构建验证（预期失败，需修复警告）**

Run: `rm -rf build && mkdir build && cd build && cmake .. && cmake --build .`
Expected: 编译失败，显示需要修复的警告

- [ ] **Step 3: 修复 ccary.h 中的函数指针类型警告**

`ccary_foreach_func` 返回 `void*` 但实际回调函数不需要返回值。修改为：

```c
typedef void (*ccary_foreach_func)(void *);
```

同时修改 `ccary.c` 中的 `print_string` 函数签名以匹配：

```c
static void print_string(void *str) {
    printf("|%s|\n", (char *)str);
}
```

- [ ] **Step 4: 再次构建验证**

Run: `cmake --build build`
Expected: 成功编译，无警告

- [ ] **Step 5: Commit**

```bash
git add CMakeLists.txt include/ccary.h src/ccary.c
git commit -m "build: 添加严格编译器警告并修复现有代码"
```

---

### Task 3: 创建 .clang-tidy 配置文件

**Files:**
- Create: `.clang-tidy`

- [ ] **Step 1: 创建配置文件**

```yaml
Checks: >
    bugprone-*,
    clang-analyzer-*,
    cppcoreguidelines-*,
    performance-*,
    portability-*,
    readability-*,
    -cppcoreguidelines-avoid-magic-numbers,
    -readability-named-parameter

WarningsAsErrors: '*'
HeaderFilterRegex: '.*'
FormatStyle: none
```

- [ ] **Step 2: 在 CMakeLists.txt 中集成 clang-tidy**

在 `CMakeLists.txt` 中添加：

```cmake
# clang-tidy integration
find_program(CLANG_TIDY_EXE NAMES clang-tidy)
if(CLANG_TIDY_EXE)
    set(CMAKE_C_CLANG_TIDY "${CLANG_TIDY_EXE}")
    message(STATUS "clang-tidy found: ${CLANG_TIDY_EXE}")
else()
    message(STATUS "clang-tidy not found. Skipping clang-tidy checks.")
endif()
```

放在编译器警告配置之后。

- [ ] **Step 3: 添加 lint 自定义目标**

在 `CMakeLists.txt` 末尾添加：

```cmake
# Lint target (runs clang-tidy manually)
if(CLANG_TIDY_EXE)
    add_custom_target(lint
        COMMAND ${CLANG_TIDY_EXE}
            ${CMAKE_SOURCE_DIR}/src/*.c
            ${CMAKE_SOURCE_DIR}/include/*.h
            --
            -I${CMAKE_SOURCE_DIR}/include
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        COMMENT "Running clang-tidy..."
    )
endif()
```

- [ ] **Step 4: 验证 clang-tidy 配置**

Run: `cd build && cmake .. && cmake --build .`
Expected: 如果安装了 clang-tidy，编译时会自动运行检查

- [ ] **Step 5: Commit**

```bash
git add .clang-tidy CMakeLists.txt
git commit -m "build: 添加 clang-tidy 配置和 lint 目标"
```

---

### Task 4: 使用 FetchContent 引入 Unity 测试框架

**Files:**
- Modify: `CMakeLists.txt`
- Create: `tests/CMakeLists.txt`

- [ ] **Step 1: 在根 CMakeLists.txt 中启用测试并添加子目录**

在根 `CMakeLists.txt` 末尾（install 之前）添加：

```cmake
# Testing
enable_testing()
add_subdirectory(tests)
```

- [ ] **Step 2: 创建 tests/CMakeLists.txt**

```cmake
include(FetchContent)

FetchContent_Declare(
    unity
    GIT_REPOSITORY https://github.com/ThrowTheSwitch/Unity.git
    GIT_TAG v2.6.0
)

FetchContent_MakeAvailable(unity)

# Test for stringplus
add_executable(test_stringplus test_stringplus.c)
target_link_libraries(test_stringplus stringplus unity)
add_test(NAME test_stringplus COMMAND test_stringplus)

# Test for ccary
add_executable(test_ccary test_ccary.c)
target_link_libraries(test_ccary stringplus unity)
add_test(NAME test_ccary COMMAND test_ccary)
```

- [ ] **Step 3: 验证 CMake 配置**

Run: `cd build && cmake ..`
Expected: 成功配置，自动下载 Unity 框架

- [ ] **Step 4: Commit**

```bash
git add CMakeLists.txt tests/CMakeLists.txt
git commit -m "build: 引入 Unity 测试框架"
```

---

### Task 5: 编写 stringplus 单元测试

**Files:**
- Create: `tests/test_stringplus.c`

- [ ] **Step 1: 创建测试文件**

```c
#include "unity.h"
#include "stringplus.h"
#include "ccary.h"

void setUp(void) {}
void tearDown(void) {}

void test_indexof_found(void) {
    TEST_ASSERT_EQUAL_size_t(0, indexof("Hello", 'H'));
    TEST_ASSERT_EQUAL_size_t(4, indexof("Hello", 'o'));
    TEST_ASSERT_EQUAL_size_t(2, indexof("Hello", 'l'));
}

void test_indexof_not_found(void) {
    TEST_ASSERT_EQUAL_size_t(NOP, indexof("Hello", 'x'));
    TEST_ASSERT_EQUAL_size_t(NOP, indexof("", 'a'));
}

void test_indexamong_found(void) {
    TEST_ASSERT_EQUAL_size_t(5, indexamong("Hello, world", ","));
    TEST_ASSERT_EQUAL_size_t(5, indexamong("Hello; world", ";:"));
    TEST_ASSERT_EQUAL_size_t(0, indexamong("Hello", "H"));
}

void test_indexamong_not_found(void) {
    TEST_ASSERT_EQUAL_size_t(NOP, indexamong("Hello", "xyz"));
    TEST_ASSERT_EQUAL_size_t(NOP, indexamong("", "abc"));
}

void test_split_normal(void) {
    pccary result = split("a-b-c", "-");
    TEST_ASSERT_NOT_NULL(result);
    TEST_ASSERT_EQUAL_size_t(3, ccary_size(result));
    TEST_ASSERT_EQUAL_STRING("a", result->data[0]);
    TEST_ASSERT_EQUAL_STRING("b", result->data[1]);
    TEST_ASSERT_EQUAL_STRING("c", result->data[2]);
    ccary_destroy(result);
}

void test_split_multiple_delimiters(void) {
    pccary result = split("8208180119-lsq_infosec", "-_");
    TEST_ASSERT_NOT_NULL(result);
    TEST_ASSERT_EQUAL_size_t(3, ccary_size(result));
    TEST_ASSERT_EQUAL_STRING("8208180119", result->data[0]);
    TEST_ASSERT_EQUAL_STRING("lsq", result->data[1]);
    TEST_ASSERT_EQUAL_STRING("infosec", result->data[2]);
    ccary_destroy(result);
}

void test_split_consecutive_delimiters(void) {
    pccary result = split("a--b", "-");
    TEST_ASSERT_NOT_NULL(result);
    TEST_ASSERT_EQUAL_size_t(2, ccary_size(result));
    TEST_ASSERT_EQUAL_STRING("a", result->data[0]);
    TEST_ASSERT_EQUAL_STRING("b", result->data[1]);
    ccary_destroy(result);
}

void test_split_empty_string(void) {
    pccary result = split("", "-");
    TEST_ASSERT_NOT_NULL(result);
    TEST_ASSERT_EQUAL_size_t(0, ccary_size(result));
    ccary_destroy(result);
}

void test_join_normal(void) {
    pccary pcca = ccary_init();
    ccary_append(pcca, "Hello");
    ccary_append(pcca, "world");
    char *result = join(pcca, ' ');
    TEST_ASSERT_EQUAL_STRING("Hello world", result);
    free(result);
    ccary_destroy(pcca);
}

void test_join_single_element(void) {
    pccary pcca = ccary_init();
    ccary_append(pcca, "Hello");
    char *result = join(pcca, ' ');
    TEST_ASSERT_EQUAL_STRING("Hello", result);
    free(result);
    ccary_destroy(pcca);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_indexof_found);
    RUN_TEST(test_indexof_not_found);
    RUN_TEST(test_indexamong_found);
    RUN_TEST(test_indexamong_not_found);
    RUN_TEST(test_split_normal);
    RUN_TEST(test_split_multiple_delimiters);
    RUN_TEST(test_split_consecutive_delimiters);
    RUN_TEST(test_split_empty_string);
    RUN_TEST(test_join_normal);
    RUN_TEST(test_join_single_element);
    return UNITY_END();
}
```

- [ ] **Step 2: 构建并运行测试**

Run: `cmake --build build && ctest --test-dir build --output-on-failure`
Expected: test_stringplus 编译成功并通过所有测试

- [ ] **Step 3: Commit**

```bash
git add tests/test_stringplus.c
git commit -m "test: 添加 stringplus 单元测试"
```

---

### Task 6: 编写 ccary 单元测试

**Files:**
- Create: `tests/test_ccary.c`

- [ ] **Step 1: 创建测试文件**

```c
#include "unity.h"
#include "ccary.h"

void setUp(void) {}
void tearDown(void) {}

void test_ccary_init_destroy(void) {
    pccary pcca = ccary_init();
    TEST_ASSERT_NOT_NULL(pcca);
    TEST_ASSERT_EQUAL_size_t(0, ccary_size(pcca));
    TEST_ASSERT_NULL(pcca->data);
    ccary_destroy(pcca);
}

void test_ccary_append(void) {
    pccary pcca = ccary_init();
    ccary_append(pcca, "Hello");
    TEST_ASSERT_EQUAL_size_t(1, ccary_size(pcca));
    TEST_ASSERT_EQUAL_STRING("Hello", pcca->data[0]);
    ccary_append(pcca, "world");
    TEST_ASSERT_EQUAL_size_t(2, ccary_size(pcca));
    TEST_ASSERT_EQUAL_STRING("world", pcca->data[1]);
    ccary_destroy(pcca);
}

void test_ccary_append_empty_string(void) {
    pccary pcca = ccary_init();
    ccary_append(pcca, "");
    TEST_ASSERT_EQUAL_size_t(1, ccary_size(pcca));
    TEST_ASSERT_EQUAL_STRING("", pcca->data[0]);
    ccary_destroy(pcca);
}

void test_ccary_clean(void) {
    pccary pcca = ccary_init();
    ccary_append(pcca, "Hello");
    ccary_append(pcca, "world");
    TEST_ASSERT_EQUAL_size_t(2, ccary_size(pcca));
    ccary_clean(pcca);
    TEST_ASSERT_EQUAL_size_t(0, ccary_size(pcca));
    TEST_ASSERT_NULL(pcca->data);
    ccary_append(pcca, "new");
    TEST_ASSERT_EQUAL_size_t(1, ccary_size(pcca));
    TEST_ASSERT_EQUAL_STRING("new", pcca->data[0]);
    ccary_destroy(pcca);
}

void test_ccary_size(void) {
    pccary pcca = ccary_init();
    TEST_ASSERT_EQUAL_size_t(0, ccary_size(pcca));
    ccary_append(pcca, "a");
    TEST_ASSERT_EQUAL_size_t(1, ccary_size(pcca));
    ccary_append(pcca, "b");
    TEST_ASSERT_EQUAL_size_t(2, ccary_size(pcca));
    ccary_destroy(pcca);
}

static int foreach_count = 0;
static void count_callback(void *str) {
    (void)str;
    foreach_count++;
}

void test_ccary_foreach(void) {
    pccary pcca = ccary_init();
    ccary_append(pcca, "a");
    ccary_append(pcca, "b");
    ccary_append(pcca, "c");
    foreach_count = 0;
    ccary_foreach(pcca, count_callback);
    TEST_ASSERT_EQUAL(3, foreach_count);
    ccary_destroy(pcca);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_ccary_init_destroy);
    RUN_TEST(test_ccary_append);
    RUN_TEST(test_ccary_append_empty_string);
    RUN_TEST(test_ccary_clean);
    RUN_TEST(test_ccary_size);
    RUN_TEST(test_ccary_foreach);
    return UNITY_END();
}
```

- [ ] **Step 2: 构建并运行测试**

Run: `cmake --build build && ctest --test-dir build --output-on-failure`
Expected: test_ccary 编译成功并通过所有测试

- [ ] **Step 3: Commit**

```bash
git add tests/test_ccary.c
git commit -m "test: 添加 ccary 单元测试"
```

---

### Task 7: 更新 AGENTS.md 添加代码质量约束

**Files:**
- Modify: `AGENTS.md`

- [ ] **Step 1: 在 AGENTS.md 末尾添加新章节**

```markdown
## 代码质量约束

- 每次修改源代码后，**必须**执行构建（`cmake --build .`），确保编译器无警告（所有警告视为错误）。
- 每次修改源代码后，**必须**执行测试（`ctest --output-on-failure`），确保所有测试通过。
- 提交代码前，**必须**通过上述两项检查。
- 如果安装了 clang-tidy，编译时会自动进行额外静态分析；也可通过 `cmake --build . --target lint` 手动运行。
```

- [ ] **Step 2: Commit**

```bash
git add AGENTS.md
git commit -m "docs: 在 AGENTS.md 中添加代码质量约束"
```

---

### Task 8: 最终验证

- [ ] **Step 1: 完整清理构建并运行所有检查**

```bash
rm -rf build
mkdir build
cd build
cmake ..
cmake --build .
ctest --output-on-failure
```

Expected:
- 编译成功，零警告
- `ctest` 运行 2 个测试套件（test_stringplus, test_ccary），全部通过
- `app` 可执行文件正常运行

- [ ] **Step 2: Commit（如需要）**

---

## 自检清单

**1. Spec coverage:**
- [x] 编译器严格警告 — Task 2
- [x] clang-tidy 配置 — Task 3
- [x] Unity 测试框架 — Task 4
- [x] test_stringplus.c — Task 5
- [x] test_ccary.c — Task 6
- [x] AGENTS.md 约束 — Task 7

**2. Placeholder scan:**
- [x] 无 TBD/TODO
- [x] 所有步骤包含具体代码和命令

**3. Type consistency:**
- [x] `ccary_foreach_func` 类型修改一致
- [x] Unity API 使用正确（`TEST_ASSERT_EQUAL_size_t` 等）
