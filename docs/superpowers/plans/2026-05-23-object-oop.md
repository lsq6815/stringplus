# Object OOP 基础设施实现计划

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** 实现 Object 基类及配套 OOP 基础设施（引用计数、虚函数表、运行时类型识别）

**Architecture:** 采用轻量级的嵌入式虚函数表设计。Object 头部包含 `isa` 指针和 `ref_count`，所有行为方法通过 `isa->vtable` 间接调用。运行时操作（retain/release/类型识别）直接以函数提供。

**Tech Stack:** C99, CMake, 自定义测试框架（test_utils.h）

---

## 文件结构

| 文件 | 说明 |
|------|------|
| `include/obj_class.h` | ObjClass 类型和 vtable 定义（子类定义时需要 include） |
| `include/object.h` | Object 基类公共 API |
| `src/object.c` | Object 基类实现 |
| `tests/test_object.c` | 引用计数、类型识别、虚函数调用测试 |
| `CMakeLists.txt` | 添加 `src/object.c` 到库源文件 |
| `tests/CMakeLists.txt` | 添加 `test_object` 测试可执行文件 |

---

### Task 1: 创建 ObjClass 类型定义

**Files:**
- Create: `include/obj_class.h`

- [ ] **Step 1: 编写 obj_class.h**

```c
#ifndef OBJ_CLASS_H
#define OBJ_CLASS_H

#include <stddef.h>

typedef struct object object;
typedef struct obj_class obj_class;

typedef struct obj_vtable {
    void (*dealloc)(object *obj);
    const char *(*describe)(const object *obj);
    int (*equals)(const object *a, const object *b);
    int (*compare)(const object *a, const object *b);
    size_t (*hash)(const object *obj);
} obj_vtable;

struct obj_class {
    const char *name;
    const obj_class *parent;
    const obj_vtable vtable;
};

/* 全局基类定义，供子类引用 */
extern const obj_class ObjectClass;

#endif
```

- [ ] **Step 2: Commit**

```bash
git add include/obj_class.h
git commit -m "feat(object): 添加 ObjClass 和 vtable 类型定义"
```

---

### Task 2: 创建 Object 公共 API 头文件

**Files:**
- Create: `include/object.h`

- [ ] **Step 1: 编写 object.h**

```c
#ifndef OBJECT_H
#define OBJECT_H

#include "obj_class.h"

/**
 * @brief 创建 Object 基类实例
 * @return 指向新对象的指针，失败返回 NULL
 */
object *object_create(void);

/**
 * @brief 增加对象引用计数
 * @param obj : 对象指针
 */
void obj_retain(object *obj);

/**
 * @brief 减少对象引用计数，归零时自动释放
 * @param obj : 对象指针
 */
void obj_release(object *obj);

/**
 * @brief 获取对象描述字符串
 * @param obj : 对象指针
 * @return 描述字符串，对象为 NULL 时返回 NULL
 */
const char *obj_describe(object *obj);

/**
 * @brief 判断两个对象是否相等
 * @param a : 对象 A
 * @param b : 对象 B
 * @return 1 表示相等，0 表示不相等，-1 表示错误
 */
int obj_equals(object *a, object *b);

/**
 * @brief 比较两个对象
 * @param a : 对象 A
 * @param b : 对象 B
 * @return 负数(a<b), 0(a==b), 正数(a>b), -1 表示错误
 */
int obj_compare(object *a, object *b);

/**
 * @brief 计算对象哈希值
 * @param obj : 对象指针
 * @return 哈希值
 */
size_t obj_hash(object *obj);

/**
 * @brief 判断对象是否属于某类或其子类
 * @param obj : 对象指针
 * @param cls : 类指针
 * @return 1 表示是，0 表示否
 */
int obj_is_kind_of_class(const object *obj, const obj_class *cls);

/**
 * @brief 判断对象是否严格属于某类
 * @param obj : 对象指针
 * @param cls : 类指针
 * @return 1 表示是，0 表示否
 */
int obj_is_member_of_class(const object *obj, const obj_class *cls);

/**
 * @brief 获取对象类名
 * @param obj : 对象指针
 * @return 类名字符串
 */
const char *obj_class_name(const object *obj);

/**
 * @brief 获取对象所属类
 * @param obj : 对象指针
 * @return 类指针
 */
const obj_class *obj_get_class(const object *obj);

#endif
```

- [ ] **Step 2: Commit**

```bash
git add include/object.h
git commit -m "feat(object): 添加 Object 公共 API 头文件"
```

---

### Task 3: 实现 Object 基类

**Files:**
- Create: `src/object.c`
- Modify: `CMakeLists.txt`（添加 `src/object.c` 到 `LIB_SRCS`）

- [ ] **Step 1: 编写 object.c**

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/object.h"
#include "../include/obj_class.h"

/* object.c 内部可访问完整定义 */
struct object {
    const obj_class *isa;
    size_t ref_count;
};

/* 基类默认实现 */
static void object_dealloc(object *obj) {
    if (obj == NULL) {
        return;
    }
    free(obj);
}

static const char *object_describe(const object *obj) {
    if (obj == NULL) {
        return NULL;
    }
    return obj->isa->name;
}

static int object_equals(const object *a, const object *b) {
    if (a == NULL || b == NULL) {
        return -1;
    }
    return a == b ? 1 : 0;
}

static int object_compare(const object *a, const object *b) {
    if (a == NULL || b == NULL) {
        return -1;
    }
    if (a < b) {
        return -1;
    }
    if (a > b) {
        return 1;
    }
    return 0;
}

static size_t object_hash(const object *obj) {
    if (obj == NULL) {
        return 0;
    }
    return (size_t)obj;
}

static const obj_vtable object_vtable = {
    .dealloc = object_dealloc,
    .describe = object_describe,
    .equals = object_equals,
    .compare = object_compare,
    .hash = object_hash,
};

const obj_class ObjectClass = {
    .name = "Object",
    .parent = NULL,
    .vtable = object_vtable,
};

object *object_create(void) {
    object *obj = malloc(sizeof(object));
    if (obj == NULL) {
        return NULL;
    }
    obj->isa = &ObjectClass;
    obj->ref_count = 1;
    return obj;
}

void obj_retain(object *obj) {
    if (obj == NULL) {
        return;
    }
    obj->ref_count++;
}

void obj_release(object *obj) {
    if (obj == NULL) {
        return;
    }
    obj->ref_count--;
    if (obj->ref_count == 0) {
        obj->isa->vtable.dealloc(obj);
    }
}

const char *obj_describe(object *obj) {
    if (obj == NULL || obj->isa == NULL) {
        return NULL;
    }
    return obj->isa->vtable.describe(obj);
}

int obj_equals(object *a, object *b) {
    if (a == NULL || b == NULL || a->isa == NULL) {
        return -1;
    }
    return a->isa->vtable.equals(a, b);
}

int obj_compare(object *a, object *b) {
    if (a == NULL || b == NULL || a->isa == NULL) {
        return -1;
    }
    return a->isa->vtable.compare(a, b);
}

size_t obj_hash(object *obj) {
    if (obj == NULL || obj->isa == NULL) {
        return 0;
    }
    return obj->isa->vtable.hash(obj);
}

int obj_is_kind_of_class(const object *obj, const obj_class *cls) {
    const obj_class *current = NULL;
    if (obj == NULL || cls == NULL || obj->isa == NULL) {
        return 0;
    }
    current = obj->isa;
    while (current != NULL) {
        if (current == cls) {
            return 1;
        }
        current = current->parent;
    }
    return 0;
}

int obj_is_member_of_class(const object *obj, const obj_class *cls) {
    if (obj == NULL || cls == NULL || obj->isa == NULL) {
        return 0;
    }
    return obj->isa == cls ? 1 : 0;
}

const char *obj_class_name(const object *obj) {
    if (obj == NULL || obj->isa == NULL) {
        return NULL;
    }
    return obj->isa->name;
}

const obj_class *obj_get_class(const object *obj) {
    if (obj == NULL) {
        return NULL;
    }
    return obj->isa;
}
```

- [ ] **Step 2: 修改 CMakeLists.txt**

将第 74 行：
```cmake
set (LIB_SRCS ./src/stringplus.c ./src/ccary.c)
```
修改为：
```cmake
set (LIB_SRCS ./src/stringplus.c ./src/ccary.c ./src/object.c)
```

- [ ] **Step 3: 构建验证**

```bash
cmake -S . -B build -G "MinGW Makefiles" -DCMAKE_C_COMPILER=gcc
cmake --build build
```

Expected: 编译成功，无警告/错误。

- [ ] **Step 4: Commit**

```bash
git add src/object.c CMakeLists.txt
git commit -m "feat(object): 实现 Object 基类（引用计数、虚函数、类型识别）"
```

---

### Task 4: 编写测试 - 引用计数

**Files:**
- Create: `tests/test_object.c`
- Modify: `tests/CMakeLists.txt`

- [ ] **Step 1: 编写引用计数测试**

在 `tests/test_object.c` 中写入：

```c
#include "test_utils.h"
#include "object.h"
#include "obj_class.h"

static int test_object_create(void) {
    object *obj = object_create();
    TEST_ASSERT_NOT_NULL(obj);
    TEST_ASSERT_EQUAL_STRING("Object", obj_class_name(obj));
    obj_release(obj);
    return 0;
}

static int test_object_retain_release(void) {
    object *obj = object_create();
    TEST_ASSERT_NOT_NULL(obj);
    
    obj_retain(obj);
    obj_retain(obj);
    obj_release(obj);
    obj_release(obj);
    /* 还有一次引用，释放不应崩溃 */
    obj_release(obj);
    return 0;
}

static int test_object_release_null(void) {
    obj_release(NULL);  /* Should not crash */
    obj_retain(NULL);   /* Should not crash */
    return 0;
}

typedef int (*test_func_t)(void);

int main(void) {
    static const test_func_t test_funcs[] = {
        test_object_create,
        test_object_retain_release,
        test_object_release_null,
    };

    static const char * const test_names[] = {
        "test_object_create",
        "test_object_retain_release",
        "test_object_release_null",
    };

    TEST_START();
    for (size_t idx = 0; idx < sizeof(test_funcs) / sizeof(test_funcs[0]); idx++) {
        RUN_TEST_NAMED(test_funcs[idx], test_names[idx]);
    }
    TEST_END();
}
```

- [ ] **Step 2: 修改 tests/CMakeLists.txt**

在文件末尾添加：
```cmake
# Test for object
add_executable(test_object test_object.c)
target_link_libraries(test_object stringplus)
add_test(NAME test_object COMMAND test_object)
```

- [ ] **Step 3: 运行测试**

```bash
cmake --build build
ctest --output-on-failure
```

Expected: 所有测试通过。

- [ ] **Step 4: Commit**

```bash
git add tests/test_object.c tests/CMakeLists.txt
git commit -m "test(object): 添加引用计数基础测试"
```

---

### Task 5: 编写测试 - 虚函数调用

**Files:**
- Modify: `tests/test_object.c`

- [ ] **Step 1: 添加虚函数测试函数**

在 `tests/test_object.c` 中，在 `test_object_release_null` 函数后添加：

```c
static int test_object_describe(void) {
    object *obj = object_create();
    TEST_ASSERT_NOT_NULL(obj);
    TEST_ASSERT_EQUAL_STRING("Object", obj_describe(obj));
    obj_release(obj);
    return 0;
}

static int test_object_describe_null(void) {
    TEST_ASSERT_NULL(obj_describe(NULL));
    return 0;
}

static int test_object_equals(void) {
    object *obj1 = object_create();
    object *obj2 = object_create();
    TEST_ASSERT_NOT_NULL(obj1);
    TEST_ASSERT_NOT_NULL(obj2);
    TEST_ASSERT_EQUAL_INT(1, obj_equals(obj1, obj1));
    TEST_ASSERT_EQUAL_INT(0, obj_equals(obj1, obj2));
    obj_release(obj1);
    obj_release(obj2);
    return 0;
}

static int test_object_equals_null(void) {
    object *obj = object_create();
    TEST_ASSERT_EQUAL_INT(-1, obj_equals(NULL, obj));
    TEST_ASSERT_EQUAL_INT(-1, obj_equals(obj, NULL));
    obj_release(obj);
    return 0;
}

static int test_object_compare(void) {
    object *obj1 = object_create();
    object *obj2 = object_create();
    TEST_ASSERT_NOT_NULL(obj1);
    TEST_ASSERT_NOT_NULL(obj2);
    TEST_ASSERT_EQUAL_INT(0, obj_compare(obj1, obj1));
    if (obj1 < obj2) {
        TEST_ASSERT_EQUAL_INT(-1, obj_compare(obj1, obj2));
        TEST_ASSERT_EQUAL_INT(1, obj_compare(obj2, obj1));
    } else {
        TEST_ASSERT_EQUAL_INT(1, obj_compare(obj1, obj2));
        TEST_ASSERT_EQUAL_INT(-1, obj_compare(obj2, obj1));
    }
    obj_release(obj1);
    obj_release(obj2);
    return 0;
}

static int test_object_hash(void) {
    object *obj = object_create();
    TEST_ASSERT_NOT_NULL(obj);
    TEST_ASSERT_EQUAL((size_t)obj, obj_hash(obj));
    TEST_ASSERT_EQUAL((size_t)0, obj_hash(NULL));
    obj_release(obj);
    return 0;
}
```

- [ ] **Step 2: 更新 main 函数测试列表**

将 `test_funcs` 和 `test_names` 数组更新为：

```c
static const test_func_t test_funcs[] = {
    test_object_create,
    test_object_retain_release,
    test_object_release_null,
    test_object_describe,
    test_object_describe_null,
    test_object_equals,
    test_object_equals_null,
    test_object_compare,
    test_object_hash,
};

static const char * const test_names[] = {
    "test_object_create",
    "test_object_retain_release",
    "test_object_release_null",
    "test_object_describe",
    "test_object_describe_null",
    "test_object_equals",
    "test_object_equals_null",
    "test_object_compare",
    "test_object_hash",
};
```

- [ ] **Step 3: 运行测试**

```bash
cmake --build build
ctest --output-on-failure
```

Expected: 所有测试通过。

- [ ] **Step 4: Commit**

```bash
git add tests/test_object.c
git commit -m "test(object): 添加虚函数调用测试（describe, equals, compare, hash）"
```

---

### Task 6: 编写测试 - 运行时类型识别

**Files:**
- Modify: `tests/test_object.c`

- [ ] **Step 1: 添加类型识别测试函数**

在 `tests/test_object.c` 中，在 `test_object_hash` 函数后添加：

```c
static int test_object_is_member_of_class(void) {
    object *obj = object_create();
    TEST_ASSERT_NOT_NULL(obj);
    TEST_ASSERT_EQUAL_INT(1, obj_is_member_of_class(obj, &ObjectClass));
    TEST_ASSERT_EQUAL_INT(0, obj_is_member_of_class(obj, NULL));
    TEST_ASSERT_EQUAL_INT(0, obj_is_member_of_class(NULL, &ObjectClass));
    obj_release(obj);
    return 0;
}

static int test_object_is_kind_of_class(void) {
    object *obj = object_create();
    TEST_ASSERT_NOT_NULL(obj);
    TEST_ASSERT_EQUAL_INT(1, obj_is_kind_of_class(obj, &ObjectClass));
    TEST_ASSERT_EQUAL_INT(0, obj_is_kind_of_class(obj, NULL));
    TEST_ASSERT_EQUAL_INT(0, obj_is_kind_of_class(NULL, &ObjectClass));
    obj_release(obj);
    return 0;
}

static int test_object_get_class(void) {
    object *obj = object_create();
    TEST_ASSERT_NOT_NULL(obj);
    TEST_ASSERT(obj_get_class(obj) == &ObjectClass);
    TEST_ASSERT_NULL(obj_get_class(NULL));
    obj_release(obj);
    return 0;
}

static int test_object_class_name(void) {
    object *obj = object_create();
    TEST_ASSERT_NOT_NULL(obj);
    TEST_ASSERT_EQUAL_STRING("Object", obj_class_name(obj));
    TEST_ASSERT_NULL(obj_class_name(NULL));
    obj_release(obj);
    return 0;
}
```

- [ ] **Step 2: 更新 main 函数测试列表**

将 `test_funcs` 和 `test_names` 数组更新为：

```c
static const test_func_t test_funcs[] = {
    test_object_create,
    test_object_retain_release,
    test_object_release_null,
    test_object_describe,
    test_object_describe_null,
    test_object_equals,
    test_object_equals_null,
    test_object_compare,
    test_object_hash,
    test_object_is_member_of_class,
    test_object_is_kind_of_class,
    test_object_get_class,
    test_object_class_name,
};

static const char * const test_names[] = {
    "test_object_create",
    "test_object_retain_release",
    "test_object_release_null",
    "test_object_describe",
    "test_object_describe_null",
    "test_object_equals",
    "test_object_equals_null",
    "test_object_compare",
    "test_object_hash",
    "test_object_is_member_of_class",
    "test_object_is_kind_of_class",
    "test_object_get_class",
    "test_object_class_name",
};
```

- [ ] **Step 3: 运行测试**

```bash
cmake --build build
ctest --output-on-failure
```

Expected: 所有测试通过。

- [ ] **Step 4: Commit**

```bash
git add tests/test_object.c
git commit -m "test(object): 添加运行时类型识别测试"
```

---

### Task 7: 验证完整测试套件

**Files:**
- 无修改，仅验证

- [ ] **Step 1: 运行完整测试套件**

```bash
cd build && ctest --output-on-failure
```

Expected: 
- test_stringplus: PASS
- test_ccary: PASS
- test_object: PASS

- [ ] **Step 2: 运行 linter**

```bash
cmake --build build --target lint
```

Expected: 无错误（clang-tidy 通过）。

- [ ] **Step 3: 最终提交（如有需要）**

如果一切正常，无需额外提交。

---

## 自审检查

### Spec 覆盖检查

| Spec 章节 | 实现任务 |
|-----------|---------|
| 3.1 Object 内存布局 | Task 3 |
| 3.2 ObjClass 类型 | Task 1 |
| 3.3 单继承模型 | 设计文档中定义，供未来子类使用 |
| 4.1 所有权规则 | Task 3 |
| 4.2 引用计数 API | Task 3, Task 4 |
| 4.3 析构链 | Task 3（object_dealloc） |
| 5.1 基类构造 | Task 3 |
| 5.2 子类构造模式 | 设计文档中定义，供未来子类使用 |
| 6.1 调用方式 | Task 3（函数 + vtable 包装） |
| 6.2 虚函数表填充 | Task 3（ObjectClass 定义） |
| 7. 运行时类型识别 | Task 3, Task 6 |

### Placeholder 扫描

- 无 "TBD", "TODO", "implement later"
- 所有步骤包含完整代码
- 所有测试包含具体断言
- 无 "Similar to Task N" 引用

### 类型一致性

- `obj_vtable` 函数签名在所有任务中一致
- `obj_class` 结构体定义一致
- 函数命名遵循 `obj_动词` / `object_动词` 规范
