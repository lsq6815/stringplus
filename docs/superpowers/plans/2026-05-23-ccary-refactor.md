# ccary C OOP 重构实现计划

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** 将 ccary 重构为符合 C OOP 规范的不透明类型，引入 capacity 字段和指数扩容，添加迭代器，所有可能失败的操作返回 int 错误码。

**Architecture:** 头文件仅暴露不完整声明 `typedef struct ccary ccary;`，实现文件包含完整结构体（size + capacity + data）。移除 pccary 别名，统一使用 `ccary *`。新增 ccary_iterator 类型。所有依赖 ccary 的代码（stringplus 和测试）同步更新。

**Tech Stack:** C, CMake, 自定义测试框架 (test_utils.h)

---

## 文件变更清单

| 文件 | 变更类型 | 说明 |
|------|----------|------|
| `include/ccary.h` | 重写 | 不透明类型、新 API、迭代器声明 |
| `src/ccary.c` | 重写 | 完整结构体、构造函数带 capacity、指数扩容、迭代器实现 |
| `tests/test_ccary.c` | 重构 | 移除 pccary，使用 ccary_get_at，新增迭代器测试 |
| `include/stringplus.h` | 修改 | pccary -> ccary* |
| `src/stringplus.c` | 修改 | pccary -> ccary*，ccary_init -> ccary_create |
| `tests/test_stringplus.c` | 重构 | pccary -> ccary*，使用 ccary_get_at |

---

### Task 1: 重写 ccary.h 头文件

**Files:**
- Modify: `include/ccary.h`

- [ ] **Step 1: 重写头文件为不透明类型 + 新 API**

```c
#ifndef CCARY_H
#define CCARY_H

#include <string.h>

/**
 * @brief 动态字符串数组（不透明类型）
 */
typedef struct ccary ccary;

/**
 * @brief 遍历回调函数类型
 */
typedef void (*ccary_foreach_func)(void *);

/**
 * @brief 迭代器（不透明类型）
 */
typedef struct ccary_iterator ccary_iterator;

/**
 * @brief 构造并初始化一个 ccary
 *
 * @param initial_capacity : 初始容量，为 0 时使用默认值
 * @return 指向新 ccary 的指针，失败返回 NULL
 */
ccary *ccary_create(size_t initial_capacity);

/**
 * @brief 销毁 ccary，释放所有空间
 *
 * @param ca : 指向 ccary 的指针
 */
void ccary_destroy(ccary *ca);

/**
 * @brief 遍历 ccary 中的每个字符串并调用 func
 * 注意：在 func 中调用 free() 会导致内存泄漏！
 *
 * @param ca   : 指向 ccary 的指针
 * @param func : 处理字符串的函数
 * @return 0 表示成功，-1 表示失败
 */
int ccary_foreach(ccary *ca, ccary_foreach_func func);

/**
 * @brief 清理 ccary 中的所有元素，保留容量
 *
 * @param ca : 指向 ccary 的指针
 * @return 0 表示成功，-1 表示失败
 */
int ccary_clean(ccary *ca);

/**
 * @brief 获取 ccary 的元素数量
 *
 * @param ca : 指向 ccary 的指针
 * @return 元素数量
 */
size_t ccary_get_size(const ccary *ca);

/**
 * @brief 获取 ccary 的当前容量
 *
 * @param ca : 指向 ccary 的指针
 * @return 当前容量
 */
size_t ccary_get_capacity(const ccary *ca);

/**
 * @brief 获取指定索引的元素
 *
 * @param ca    : 指向 ccary 的指针
 * @param index : 索引
 * @return 指向字符串的指针，越界返回 NULL
 */
const char *ccary_get_at(const ccary *ca, size_t index);

/**
 * @brief 将 str 追加到 ccary
 *
 * @param ca  : 指向 ccary 的指针
 * @param str : C 风格字符串
 * @return 0 表示成功，-1 表示失败
 */
int ccary_append(ccary *ca, const char *str);

/**
 * @brief 显示 ccary 的内容
 *
 * @param ca : 指向 ccary 的指针
 */
void ccary_display(ccary *ca);

/**
 * @brief 创建迭代器
 *
 * @param ca : 指向 ccary 的指针
 * @return 指向新迭代器的指针，失败返回 NULL
 */
ccary_iterator *ccary_iterator_create(const ccary *ca);

/**
 * @brief 销毁迭代器
 *
 * @param it : 指向迭代器的指针
 */
void ccary_iterator_destroy(ccary_iterator *it);

/**
 * @brief 检查迭代器是否还有下一个元素
 *
 * @param it : 指向迭代器的指针
 * @return 1 表示有，0 表示没有，-1 表示错误
 */
int ccary_iterator_has_next(const ccary_iterator *it);

/**
 * @brief 获取迭代器的下一个元素
 *
 * @param it : 指向迭代器的指针
 * @return 指向字符串的指针，没有下一个时返回 NULL
 */
const char *ccary_iterator_next(ccary_iterator *it);

#endif
```

- [ ] **Step 2: 提交**

```bash
git add include/ccary.h
git commit -m "feat(ccary): 重写头文件，引入不透明类型和新 API"
```

---

### Task 2: 重写 ccary.c 实现

**Files:**
- Modify: `src/ccary.c`

- [ ] **Step 1: 实现完整结构体和所有函数**

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/ccary.h"

#define CCARY_DEFAULT_CAPACITY 16

struct ccary {
    size_t size;
    size_t capacity;
    char **data;
};

struct ccary_iterator {
    const ccary *ca;
    size_t current;
};

ccary *ccary_create(size_t initial_capacity) {
    ccary *ca = malloc(sizeof(ccary));
    if (ca == NULL) {
        return NULL;
    }

    ca->size = 0;
    ca->capacity = (initial_capacity == 0) ? CCARY_DEFAULT_CAPACITY : initial_capacity;
    ca->data = malloc(sizeof(char *) * ca->capacity);
    if (ca->data == NULL) {
        free(ca);
        return NULL;
    }

    return ca;
}

void ccary_destroy(ccary *ca) {
    size_t idx = 0;
    if (ca == NULL) {
        return;
    }

    for (idx = 0; idx < ca->size; idx++) {
        free(ca->data[idx]);
    }

    free(ca->data);
    free(ca);
}

int ccary_foreach(ccary *ca, ccary_foreach_func func) {
    size_t idx = 0;
    if (ca == NULL || func == NULL) {
        return -1;
    }

    for (idx = 0; idx < ca->size; idx++) {
        func(ca->data[idx]);
    }

    return 0;
}

int ccary_clean(ccary *ca) {
    size_t idx = 0;
    if (ca == NULL) {
        return -1;
    }

    for (idx = 0; idx < ca->size; idx++) {
        free(ca->data[idx]);
    }

    ca->size = 0;
    return 0;
}

size_t ccary_get_size(const ccary *ca) {
    if (ca == NULL) {
        return 0;
    }
    return ca->size;
}

size_t ccary_get_capacity(const ccary *ca) {
    if (ca == NULL) {
        return 0;
    }
    return ca->capacity;
}

const char *ccary_get_at(const ccary *ca, size_t index) {
    if (ca == NULL || index >= ca->size) {
        return NULL;
    }
    return ca->data[index];
}

int ccary_append(ccary *ca, const char *str) {
    char **tmp = NULL;
    size_t new_capacity = 0;

    if (ca == NULL || str == NULL) {
        return -1;
    }

    if (ca->size >= ca->capacity) {
        new_capacity = (ca->capacity == 0) ? 1 : ca->capacity * 2;
        tmp = realloc(ca->data, sizeof(char *) * new_capacity);
        if (tmp == NULL) {
            return -1;
        }
        ca->data = tmp;
        ca->capacity = new_capacity;
    }

    ca->data[ca->size] = malloc(strlen(str) + 1);
    if (ca->data[ca->size] == NULL) {
        return -1;
    }

    memcpy(ca->data[ca->size], str, strlen(str) + 1);
    ca->size += 1;
    return 0;
}

static void print_string(void *str) {
    printf("|%s|\n", (char *)str);
}

void ccary_display(ccary *ca) {
    if (ca == NULL) {
        return;
    }
    ccary_foreach(ca, print_string);
    printf("Total: %zu\n", ccary_get_size(ca));
}

ccary_iterator *ccary_iterator_create(const ccary *ca) {
    ccary_iterator *it = NULL;
    if (ca == NULL) {
        return NULL;
    }

    it = malloc(sizeof(ccary_iterator));
    if (it == NULL) {
        return NULL;
    }

    it->ca = ca;
    it->current = 0;
    return it;
}

void ccary_iterator_destroy(ccary_iterator *it) {
    free(it);
}

int ccary_iterator_has_next(const ccary_iterator *it) {
    if (it == NULL || it->ca == NULL) {
        return -1;
    }
    return it->current < it->ca->size ? 1 : 0;
}

const char *ccary_iterator_next(ccary_iterator *it) {
    if (it == NULL || it->ca == NULL || it->current >= it->ca->size) {
        return NULL;
    }
    return it->ca->data[it->current++];
}
```

- [ ] **Step 2: 提交**

```bash
git add src/ccary.c
git commit -m "feat(ccary): 实现不透明类型、指数扩容和迭代器"
```

---

### Task 3: 重构 tests/test_ccary.c

**Files:**
- Modify: `tests/test_ccary.c`

- [ ] **Step 1: 重写测试，使用新 API 并新增测试**

```c
#include "test_utils.h"
#include "ccary.h"

static int test_ccary_create_destroy(void) {
    ccary *ca = ccary_create(0);
    TEST_ASSERT_NOT_NULL(ca);
    TEST_ASSERT_EQUAL_size_t(0, ccary_get_size(ca));
    TEST_ASSERT_EQUAL_size_t(16, ccary_get_capacity(ca));
    ccary_destroy(ca);
    return 0;
}

static int test_ccary_create_with_capacity(void) {
    ccary *ca = ccary_create(8);
    TEST_ASSERT_NOT_NULL(ca);
    TEST_ASSERT_EQUAL_size_t(0, ccary_get_size(ca));
    TEST_ASSERT_EQUAL_size_t(8, ccary_get_capacity(ca));
    ccary_destroy(ca);
    return 0;
}

static int test_ccary_append(void) {
    ccary *ca = ccary_create(0);
    TEST_ASSERT_EQUAL_INT(0, ccary_append(ca, "Hello"));
    TEST_ASSERT_EQUAL_size_t(1, ccary_get_size(ca));
    TEST_ASSERT_EQUAL_STRING("Hello", ccary_get_at(ca, 0));
    TEST_ASSERT_EQUAL_INT(0, ccary_append(ca, "world"));
    TEST_ASSERT_EQUAL_size_t(2, ccary_get_size(ca));
    TEST_ASSERT_EQUAL_STRING("world", ccary_get_at(ca, 1));
    ccary_destroy(ca);
    return 0;
}

static int test_ccary_append_empty_string(void) {
    ccary *ca = ccary_create(0);
    TEST_ASSERT_EQUAL_INT(0, ccary_append(ca, ""));
    TEST_ASSERT_EQUAL_size_t(1, ccary_get_size(ca));
    TEST_ASSERT_EQUAL_STRING("", ccary_get_at(ca, 0));
    ccary_destroy(ca);
    return 0;
}

static int test_ccary_clean(void) {
    ccary *ca = ccary_create(0);
    TEST_ASSERT_EQUAL_INT(0, ccary_append(ca, "Hello"));
    TEST_ASSERT_EQUAL_INT(0, ccary_append(ca, "world"));
    TEST_ASSERT_EQUAL_size_t(2, ccary_get_size(ca));
    TEST_ASSERT_EQUAL_INT(0, ccary_clean(ca));
    TEST_ASSERT_EQUAL_size_t(0, ccary_get_size(ca));
    TEST_ASSERT_EQUAL_size_t(16, ccary_get_capacity(ca));
    TEST_ASSERT_EQUAL_INT(0, ccary_append(ca, "new"));
    TEST_ASSERT_EQUAL_size_t(1, ccary_get_size(ca));
    TEST_ASSERT_EQUAL_STRING("new", ccary_get_at(ca, 0));
    ccary_destroy(ca);
    return 0;
}

static int test_ccary_size(void) {
    ccary *ca = ccary_create(0);
    TEST_ASSERT_EQUAL_size_t(0, ccary_get_size(ca));
    TEST_ASSERT_EQUAL_INT(0, ccary_append(ca, "a"));
    TEST_ASSERT_EQUAL_size_t(1, ccary_get_size(ca));
    TEST_ASSERT_EQUAL_INT(0, ccary_append(ca, "b"));
    TEST_ASSERT_EQUAL_size_t(2, ccary_get_size(ca));
    ccary_destroy(ca);
    return 0;
}

static int g_foreach_count = 0;  /* NOLINT(cppcoreguidelines-avoid-non-const-global-variables) */
static void count_callback(void *str) {
    (void)str;
    g_foreach_count++;
}

static int test_ccary_foreach(void) {
    ccary *ca = ccary_create(0);
    TEST_ASSERT_EQUAL_INT(0, ccary_append(ca, "a"));
    TEST_ASSERT_EQUAL_INT(0, ccary_append(ca, "b"));
    TEST_ASSERT_EQUAL_INT(0, ccary_append(ca, "c"));
    g_foreach_count = 0;
    TEST_ASSERT_EQUAL_INT(0, ccary_foreach(ca, count_callback));
    TEST_ASSERT_EQUAL(3, g_foreach_count);
    ccary_destroy(ca);
    return 0;
}

static int test_ccary_get_at_out_of_bounds(void) {
    ccary *ca = ccary_create(0);
    TEST_ASSERT_NULL(ccary_get_at(ca, 0));
    TEST_ASSERT_EQUAL_INT(0, ccary_append(ca, "a"));
    TEST_ASSERT_NULL(ccary_get_at(ca, 1));
    ccary_destroy(ca);
    return 0;
}

static int test_ccary_capacity_growth(void) {
    ccary *ca = ccary_create(2);
    TEST_ASSERT_EQUAL_size_t(2, ccary_get_capacity(ca));
    TEST_ASSERT_EQUAL_INT(0, ccary_append(ca, "a"));
    TEST_ASSERT_EQUAL_INT(0, ccary_append(ca, "b"));
    TEST_ASSERT_EQUAL_size_t(2, ccary_get_size(ca));
    TEST_ASSERT_EQUAL_size_t(2, ccary_get_capacity(ca));
    TEST_ASSERT_EQUAL_INT(0, ccary_append(ca, "c"));
    TEST_ASSERT_EQUAL_size_t(4, ccary_get_capacity(ca));
    ccary_destroy(ca);
    return 0;
}

static int test_ccary_iterator(void) {
    ccary *ca = ccary_create(0);
    ccary_iterator *it = NULL;
    TEST_ASSERT_EQUAL_INT(0, ccary_append(ca, "a"));
    TEST_ASSERT_EQUAL_INT(0, ccary_append(ca, "b"));
    TEST_ASSERT_EQUAL_INT(0, ccary_append(ca, "c"));

    it = ccary_iterator_create(ca);
    TEST_ASSERT_NOT_NULL(it);
    TEST_ASSERT_EQUAL(1, ccary_iterator_has_next(it));
    TEST_ASSERT_EQUAL_STRING("a", ccary_iterator_next(it));
    TEST_ASSERT_EQUAL(1, ccary_iterator_has_next(it));
    TEST_ASSERT_EQUAL_STRING("b", ccary_iterator_next(it));
    TEST_ASSERT_EQUAL(1, ccary_iterator_has_next(it));
    TEST_ASSERT_EQUAL_STRING("c", ccary_iterator_next(it));
    TEST_ASSERT_EQUAL(0, ccary_iterator_has_next(it));
    TEST_ASSERT_NULL(ccary_iterator_next(it));

    ccary_iterator_destroy(it);
    ccary_destroy(ca);
    return 0;
}

typedef int (*test_func_t)(void);

int main(void) {
    static const test_func_t test_funcs[] = {
        test_ccary_create_destroy,
        test_ccary_create_with_capacity,
        test_ccary_append,
        test_ccary_append_empty_string,
        test_ccary_clean,
        test_ccary_size,
        test_ccary_foreach,
        test_ccary_get_at_out_of_bounds,
        test_ccary_capacity_growth,
        test_ccary_iterator,
    };

    static const char * const test_names[] = {
        "test_ccary_create_destroy",
        "test_ccary_create_with_capacity",
        "test_ccary_append",
        "test_ccary_append_empty_string",
        "test_ccary_clean",
        "test_ccary_size",
        "test_ccary_foreach",
        "test_ccary_get_at_out_of_bounds",
        "test_ccary_capacity_growth",
        "test_ccary_iterator",
    };

    TEST_START();
    for (size_t idx = 0; idx < sizeof(test_funcs) / sizeof(test_funcs[0]); idx++) {
        RUN_TEST_NAMED(test_funcs[idx], test_names[idx]);
    }
    TEST_END();
}
```

- [ ] **Step 2: 构建并运行 ccary 测试**

Run:
```bash
cmake --build build
cd build && ./tests/test_ccary
```

Expected: 所有测试通过

- [ ] **Step 3: 提交**

```bash
git add tests/test_ccary.c
git commit -m "test(ccary): 重构测试，使用新 API 并新增迭代器测试"
```

---

### Task 4: 更新 stringplus.h 和 stringplus.c

**Files:**
- Modify: `include/stringplus.h`
- Modify: `src/stringplus.c`

- [ ] **Step 1: 更新 stringplus.h 中的类型签名**

在 `include/stringplus.h` 中：
- 将 `pccary split(const char *str, const char *delims);` 改为 `ccary *split(const char *str, const char *delims);`
- 将 `char* join(pccary strset, char jch);` 改为 `char* join(ccary *strset, char jch);`

- [ ] **Step 2: 更新 stringplus.c 中的实现**

在 `src/stringplus.c` 中：
- 将 `pccary split(const char *str, const char *delims)` 改为 `ccary *split(const char *str, const char *delims)`
- 将 `pccary result = ccary_init();` 改为 `ccary *result = ccary_create(0);`
- 将 `char* join(pccary strset, char jch)` 改为 `char* join(ccary *strset, char jch)`
- 将 `strset->size` 改为 `ccary_get_size(strset)`
- 将 `strset->data[idx]` 改为 `ccary_get_at(strset, idx)`

注意：`join` 函数中使用 `ccary_get_size` 和 `ccary_get_at` 代替直接访问内部字段。

```c
ccary *split(const char *str, const char *delims) {
    ccary *result = ccary_create(0);
    size_t idx = 0;
    size_t len = 0;
    while ((len = indexamong(str + idx, delims)) != NOP) {
        if (len > 0) {
            char *tmp = malloc(len + 1);
            strncpy(tmp, str + idx, len);
            tmp[len] = '\0';
            ccary_append(result, tmp);
            free(tmp);
        }
        idx += len + 1;
    }

    len = strlen(str) - idx;
    if (len > 0) {
        char *tmp = malloc(len + 1);
        strncpy(tmp, str + idx, len);
        tmp[len] = '\0';
        ccary_append(result, tmp);
        free(tmp);
    }

    return result;
}

char* join(ccary *strset, char jch) {
    size_t idx = 0;
    size_t len = 0;
    size_t cur = 0;
    size_t size = ccary_get_size(strset);
    char *result = NULL;

    for (idx = 0; idx < size; idx++) {
        len += strlen(ccary_get_at(strset, idx));
    }
    /* 需要添加的 jch 数量 */
    if (size > 0) {
        len += size - 1;
    }
    result = malloc(len + 1);

    for (idx = 0, cur = 0; idx < size; idx++) {
        const char *str = ccary_get_at(strset, idx);
        size_t slen = strlen(str);
        memcpy(result + cur, str, slen + 1);
        cur += slen;
        result[cur] = jch;
        cur++;
    }
    result[len] = '\0';
    return result;
}
```

- [ ] **Step 3: 提交**

```bash
git add include/stringplus.h src/stringplus.c
git commit -m "refactor(stringplus): 适配新的 ccary API，移除 pccary 使用"
```

---

### Task 5: 重构 tests/test_stringplus.c

**Files:**
- Modify: `tests/test_stringplus.c`

- [ ] **Step 1: 更新测试代码**

在 `tests/test_stringplus.c` 中：
- 将所有 `pccary` 改为 `ccary *`
- 将 `ccary_init()` 改为 `ccary_create(0)`
- 将 `result->data[0]` 等直接访问改为 `ccary_get_at(result, 0)`

```c
static int test_split_normal(void) {
    ccary *result = split("a-b-c", "-");
    TEST_ASSERT_NOT_NULL(result);
    TEST_ASSERT_EQUAL_size_t(3, ccary_get_size(result));
    TEST_ASSERT_EQUAL_STRING("a", ccary_get_at(result, 0));
    TEST_ASSERT_EQUAL_STRING("b", ccary_get_at(result, 1));
    TEST_ASSERT_EQUAL_STRING("c", ccary_get_at(result, 2));
    ccary_destroy(result);
    return 0;
}

static int test_split_multiple_delimiters(void) {
    ccary *result = split("8208180119-lsq_infosec", "-_");
    TEST_ASSERT_NOT_NULL(result);
    TEST_ASSERT_EQUAL_size_t(3, ccary_get_size(result));
    TEST_ASSERT_EQUAL_STRING("8208180119", ccary_get_at(result, 0));
    TEST_ASSERT_EQUAL_STRING("lsq", ccary_get_at(result, 1));
    TEST_ASSERT_EQUAL_STRING("infosec", ccary_get_at(result, 2));
    ccary_destroy(result);
    return 0;
}

static int test_split_consecutive_delimiters(void) {
    ccary *result = split("a--b", "-");
    TEST_ASSERT_NOT_NULL(result);
    TEST_ASSERT_EQUAL_size_t(2, ccary_get_size(result));
    TEST_ASSERT_EQUAL_STRING("a", ccary_get_at(result, 0));
    TEST_ASSERT_EQUAL_STRING("b", ccary_get_at(result, 1));
    ccary_destroy(result);
    return 0;
}

static int test_split_empty_string(void) {
    ccary *result = split("", "-");
    TEST_ASSERT_NOT_NULL(result);
    TEST_ASSERT_EQUAL_size_t(0, ccary_get_size(result));
    ccary_destroy(result);
    return 0;
}

static int test_join_normal(void) {
    ccary *ca = ccary_create(0);
    ccary_append(ca, "Hello");
    ccary_append(ca, "world");
    char *result = join(ca, ' ');
    TEST_ASSERT_EQUAL_STRING("Hello world", result);
    free(result);
    ccary_destroy(ca);
    return 0;
}

static int test_join_single_element(void) {
    ccary *ca = ccary_create(0);
    ccary_append(ca, "Hello");
    char *result = join(ca, ' ');
    TEST_ASSERT_EQUAL_STRING("Hello", result);
    free(result);
    ccary_destroy(ca);
    return 0;
}
```

- [ ] **Step 2: 构建并运行 stringplus 测试**

Run:
```bash
cmake --build build
cd build && ./tests/test_stringplus
```

Expected: 所有测试通过

- [ ] **Step 3: 提交**

```bash
git add tests/test_stringplus.c
git commit -m "test(stringplus): 重构测试，适配新的 ccary API"
```

---

### Task 6: 全量构建与测试

- [ ] **Step 1: 完整构建**

Run:
```bash
cmake --build build
```

Expected: 编译成功，无警告（`-Werror`）

- [ ] **Step 2: 运行所有测试**

Run:
```bash
ctest --output-on-failure
```

Expected: 所有测试通过

- [ ] **Step 3: 检查 lint（如果可用）**

Run:
```bash
cmake --build build --target lint
```

Expected: 无错误或警告（clang-tidy 可能未安装，如不可用则跳过）

- [ ] **Step 4: 最终提交**

```bash
git add .
git commit -m "feat(ccary): 完成 C OOP 重构"
```

---

## 自检清单

### Spec 覆盖检查

| 需求 | 对应任务 |
|------|----------|
| 不透明类型 | Task 1, Task 2 |
| 移除 pccary | Task 1, Task 3, Task 4, Task 5 |
| 构造函数带 capacity | Task 2 |
| 析构函数处理 NULL | Task 2 |
| 返回 int 错误码 | Task 2 |
| 指数扩容 | Task 2 |
| Getter 访问器 | Task 2 |
| 迭代器模式 | Task 1, Task 2, Task 3 |
| 测试重构 | Task 3, Task 5 |
| stringplus 适配 | Task 4 |

### Placeholder 扫描
- 无 "TBD", "TODO", "implement later"
- 无 "add appropriate error handling" 等模糊描述
- 所有步骤包含完整代码

### 类型一致性
- `ccary *` 统一使用
- `ccary_create` 替代 `ccary_init`
- `ccary_get_size` 替代 `ccary_size`
- `ccary_get_at` 用于元素访问
