# 测试框架迁移实现计划

> **对于 agentic workers：** 必须使用 superpowers:subagent-driven-development (推荐) 或 superpowers:executing-plans 来逐任务实现此计划。步骤使用 checkbox (`- [ ]`) 语法跟踪。

**目标：** 将自定义测试宏（`test_utils.h`）替换为 Unity 测试框架，纳入仓库

**架构：** 在 `tests/unity/` 目录放置 Unity 源码（`unity.c` + `unity.h`），编译为 CMake OBJECT 库供所有测试目标链接。修改 3 个测试文件，替换断言宏和 runner。

**技术栈：** C99, CMake 3.10+, Unity Test Framework (投掷开关/Unity)

---

### 任务 1：引入 Unity 框架源文件

**文件：**
- 创建：`tests/unity/unity.c`
- 创建：`tests/unity/unity.h`
- 修改：`tests/CMakeLists.txt`

- [ ] **步骤 1：创建 `tests/unity/` 目录**

```pwsh
New-Item -ItemType Directory -Path "tests\unity"
```

- [ ] **步骤 2：从 GitHub 下载 Unity 源码**

```pwsh
Invoke-WebRequest -Uri "https://raw.githubusercontent.com/ThrowTheSwitch/Unity/v2.6.0/src/unity.c" -OutFile "tests/unity/unity.c"
Invoke-WebRequest -Uri "https://raw.githubusercontent.com/ThrowTheSwitch/Unity/v2.6.0/src/unity_internals.h" -OutFile "tests/unity/unity_internals.h"
Invoke-WebRequest -Uri "https://raw.githubusercontent.com/ThrowTheSwitch/Unity/v2.6.0/src/unity.h" -OutFile "tests/unity/unity.h"
```

- [ ] **步骤 3：更新 `tests/CMakeLists.txt`**

```cmake
# Unity test framework (object library, avoids recompilation)
add_library(unity_obj OBJECT unity/unity.c)
target_include_directories(unity_obj PUBLIC unity)
target_compile_options(unity_obj PRIVATE -Wno-unused-parameter -Wno-sign-compare)

# Test for stringplus
add_executable(test_stringplus test_stringplus.c)
target_link_libraries(test_stringplus stringplus unity_obj)
add_test(NAME test_stringplus COMMAND test_stringplus)

# Test for ccary
add_executable(test_ccary test_ccary.c)
target_link_libraries(test_ccary stringplus unity_obj)
add_test(NAME test_ccary COMMAND test_ccary)

# Test for object
add_executable(test_object test_object.c)
target_link_libraries(test_object stringplus unity_obj)
add_test(NAME test_object COMMAND test_object)
```

- [ ] **步骤 4：构建验证 Unity 编译通过**

```pwsh
cmake --build build --target unity_obj
```

- [ ] **步骤 5：提交**

```bash
git add tests/unity/ tests/CMakeLists.txt
git commit -m "feat(test): 引入 Unity 测试框架 v2.6.0"
```

---

### 任务 2：迁移 `test_stringplus.c`

**文件：**
- 修改：`tests/test_stringplus.c`

- [ ] **步骤 1：替换 include 和移除辅助声明**

将：
```c
#include "test_utils.h"
#include "stringplus.h"
#include "ccary.h"
```
改为：
```c
#include "unity.h"
#include "stringplus.h"
#include "ccary.h"
```

- [ ] **步骤 2：修改测试函数签名和返回语句**

将所有 `static int test_xxx(void)` 改为 `static void test_xxx(void)`。
删除所有 `return 0;`。

- [ ] **步骤 3：替换断言宏**

| 旧宏 | 新宏 | 说明 |
|------|------|------|
| `TEST_ASSERT_EQUAL_size_t` | `TEST_ASSERT_EQUAL` | Unity 通用比较按类型自动分发，无需特化 |
| `TEST_ASSERT_EQUAL_STRING` | `TEST_ASSERT_EQUAL_STRING` | 同名 |
| `TEST_ASSERT_NOT_NULL` | `TEST_ASSERT_NOT_NULL` | 同名 |
| `TEST_ASSERT_NULL` | `TEST_ASSERT_NULL` | 同名 |
| `TEST_ASSERT_EQUAL_INT` | `TEST_ASSERT_EQUAL_INT` | 同名 |
| `TEST_ASSERT` | `TEST_ASSERT_TRUE` | 语义等价 |
| `TEST_ASSERT_EQUAL` | `TEST_ASSERT_EQUAL` | Unity 通用比较按类型自动分发 |

- [ ] **步骤 4：修改 runner**

移除：
```c
typedef int (*test_func_t)(void);
```

移除 `test_funcs[]` 和 `test_names[]` 数组。

将 `main()` 替换为：
```c
void setUp(void) {}
void tearDown(void) {}

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

- [ ] **步骤 5：构建验证**

```pwsh
cmake --build build --target test_stringplus 2>&1
```

- [ ] **步骤 6：运行测试**

```bash
ctest -R test_stringplus --output-on-failure
```

- [ ] **步骤 7：提交**

```bash
git add tests/test_stringplus.c
git commit -m "test: 迁移 test_stringplus 到 Unity 框架"
```

---

### 任务 3：迁移 `test_ccary.c`

**文件：**
- 修改：`tests/test_ccary.c`

- [ ] **步骤 1：替换 include**

将 `#include "test_utils.h"` 改为 `#include "unity.h"`。

- [ ] **步骤 2：修改测试函数签名**

将所有 `static int test_xxx(void)` 改为 `static void test_xxx(void)`。
删除所有 `return 0;`。

- [ ] **步骤 3：替换断言宏**

同上表。

- [ ] **步骤 4：修改 runner**

移除 `typedef int (*test_func_t)(void);`、`test_funcs[]` 和 `test_names[]` 数组。

将 `main()` 替换为：
```c
void setUp(void) {}
void tearDown(void) {}

int main(void) {
    UNITY_BEGIN();

    RUN_TEST(test_ccary_create_destroy);
    RUN_TEST(test_ccary_create_with_capacity);
    RUN_TEST(test_ccary_append);
    RUN_TEST(test_ccary_append_empty_string);
    RUN_TEST(test_ccary_clean);
    RUN_TEST(test_ccary_size);
    RUN_TEST(test_ccary_foreach);
    RUN_TEST(test_ccary_get_at_out_of_bounds);
    RUN_TEST(test_ccary_capacity_growth);
    RUN_TEST(test_ccary_null_params);
    RUN_TEST(test_ccary_iterator_empty);
    RUN_TEST(test_ccary_destroy_null);
    RUN_TEST(test_ccary_foreach_empty);
    RUN_TEST(test_ccary_iterator);

    return UNITY_END();
}
```

- [ ] **步骤 5：构建并运行测试**

```pwsh
cmake --build build --target test_ccary 2>&1
ctest -R test_ccary --output-on-failure
```

- [ ] **步骤 6：提交**

```bash
git add tests/test_ccary.c
git commit -m "test: 迁移 test_ccary 到 Unity 框架"
```

---

### 任务 4：迁移 `test_object.c`

**文件：**
- 修改：`tests/test_object.c`

- [ ] **步骤 1：替换 include**

将 `#include "test_utils.h"` 改为 `#include "unity.h"`。

- [ ] **步骤 2：修改测试函数签名**

将所有 `static int test_xxx(void)` 改为 `static void test_xxx(void)`。
删除所有 `return 0;`。

- [ ] **步骤 3：替换断言宏**

同上表。注意 `TEST_ASSERT_EQUAL((size_t)obj, obj_hash(obj))` 改为 `TEST_ASSERT_EQUAL((size_t)obj, obj_hash(obj))`（参数类型不变，Unity 通用比较按类型自动分发）。

- [ ] **步骤 4：修改 runner**

移除 `typedef int (*test_func_t)(void);`、`test_funcs[]` 和 `test_names[]` 数组。

将 `main()` 替换为：
```c
void setUp(void) {}
void tearDown(void) {}

int main(void) {
    UNITY_BEGIN();

    RUN_TEST(test_object_create);
    RUN_TEST(test_object_retain_release);
    RUN_TEST(test_object_release_null);
    RUN_TEST(test_object_describe);
    RUN_TEST(test_object_describe_null);
    RUN_TEST(test_object_equals);
    RUN_TEST(test_object_equals_null);
    RUN_TEST(test_object_compare);
    RUN_TEST(test_object_hash);
    RUN_TEST(test_object_is_member_of_class);
    RUN_TEST(test_object_is_kind_of_class);
    RUN_TEST(test_object_get_class);
    RUN_TEST(test_object_class_name);

    return UNITY_END();
}
```

- [ ] **步骤 5：构建并运行测试**

```pwsh
cmake --build build --target test_object 2>&1
ctest -R test_object --output-on-failure
```

- [ ] **步骤 6：提交**

```bash
git add tests/test_object.c
git commit -m "test: 迁移 test_object 到 Unity 框架"
```

---

### 任务 5：清理自定义测试代码

**文件：**
- 删除：`tests/test_utils.h`

- [ ] **步骤 1：删除 `test_utils.h`**

```pwsh
Remove-Item -LiteralPath "tests\test_utils.h"
```

- [ ] **步骤 2：运行全部测试确认一切正常**

```pwsh
ctest --output-on-failure
```

预期：全部 3 个测试套件通过。

- [ ] **步骤 3：提交**

```bash
git rm tests/test_utils.h
git commit -m "test: 移除自定义 test_utils.h，完成 Unity 迁移"
```
