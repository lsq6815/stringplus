# C 语言面向对象编码规范（C-OOP-Coding-Standard）

日期：2026-05-23

## 概述

本规范定义在 stringplus 项目中使用 C 语言实现面向对象封装的标准实践。所有新增的结构体封装必须遵循本规范，现有代码（如 `ccary`）应逐步按此规范重构。

## 第1章：类型系统与内存管理

### 1.1 不透明类型（Opaque Type）

头文件中只暴露类型的不完整声明，结构体定义仅在 `.c` 文件中。

```c
/* object.h */
typedef struct object object;
```

```c
/* object.c */
struct object {
    int field;
    char *data;
};
```

### 1.2 禁止值类型别名

只保留指针类型别名，禁止定义值类型别名。值拷贝会导致双重释放和未定义行为。

```c
/* 正确 */
typedef struct object object;

/* 错误 */
typedef struct object obj_t;  /* 值拷贝不安全 */
```

### 1.3 所有权模型

- 构造函数返回的指针由调用者拥有
- `*_destroy()` 必须释放对象及其管理的所有资源
- 禁止浅拷贝（除非显式设计为引用语义）

## 第2章：构造函数与析构函数模式

### 2.1 命名规范

- **构造函数**：`类型名_create()`、`类型名_init()` 或 `类型名_new()`
- **析构函数**：`类型名_destroy()` 或 `类型名_free()`

### 2.2 构造函数要求

- 分配对象内存
- 初始化所有字段为有效状态
- 返回 `NULL` 表示失败
- 失败时清理已分配的资源

```c
object *object_create(void) {
    object *obj = malloc(sizeof(object));
    if (obj == NULL) return NULL;
    
    obj->data = malloc(INITIAL_SIZE);
    if (obj->data == NULL) {
        free(obj);
        return NULL;
    }
    
    obj->field = 0;
    return obj;
}
```

### 2.3 析构函数要求

- 释放对象及其管理的所有资源
- 处理 `NULL` 参数（静默返回）

```c
void object_destroy(object *obj) {
    if (obj == NULL) return;
    free(obj->data);
    free(obj);
}
```

## 第3章：封装与访问控制

### 3.1 信息隐藏

- 所有字段必须隐藏在 `.c` 文件中
- 用户只能通过 getter/setter 访问内部状态

### 3.2 访问器命名

- **Getter**：`类型名_get_字段名()`，返回 `const` 指针或值
- **Setter**：`类型名_set_字段名()`，深拷贝输入值

```c
const char *object_get_data(const object *obj);
int object_set_data(object *obj, const char *data);
```

### 3.3 禁止直接暴露可变成员

如必须暴露数组，提供 `类型名_get_count()` 和 `类型名_get_at()` 代替直接访问。

## 第4章：方法组织与命名空间

### 4.1 函数前缀

所有公共 API 必须以 `类型名_` 为前缀，模拟命名空间。

```c
/* 正确 */
void ccary_append(ccary *ca, const char *str);

/* 错误 */
void append_string(ccary *ca, const char *str);  /* 缺少前缀 */
```

### 4.2 私有函数

内部辅助函数使用 `static`，不加前缀。

### 4.3 函数指针表（可选）

需要多态时，在结构体中包含函数指针表。

```c
struct object {
    struct object_vtable *vtbl;
    void *data;
};

struct object_vtable {
    void (*destroy)(object *);
    const char *(*to_string)(const object *);
};
```

## 第5章：继承与多态模拟

### 5.1 结构体嵌套

子类型在第一个字段嵌入父类型，确保地址兼容。

```c
struct child {
    struct parent base;  /* 必须是第一个字段 */
    int extra_field;
};

/* 允许向上转换 */
parent *p = (parent *)&child_instance;
```

### 5.2 禁止直接访问父类字段

通过父类提供的 getter/setter 访问，保持封装。

## 第6章：错误处理策略

### 6.1 返回码

方法返回 `int`，`0` 表示成功，`-1` 表示失败。

### 6.2 错误信息查询

提供 `类型名_get_error()` 或全局错误码获取详细错误信息。

### 6.3 资源安全

失败时必须保持对象处于一致状态，已分配的资源已释放。

### 6.4 NULL 检查

所有公共 API 必须在入口处检查参数是否为 `NULL`。

```c
int object_set_data(object *obj, const char *data) {
    if (obj == NULL || data == NULL) return -1;
    /* ... */
}
```

## 第7章：命名与代码风格

### 7.1 文件命名

- 实现文件：`类型名.c`
- 头文件：`类型名.h`

### 7.2 类型命名

小写 + 下划线（如 `string_builder`、`dynamic_array`）。

### 7.3 函数命名

`类型名_动词_名词`（如 `ccary_append_string`、`string_builder_clear`）。

### 7.4 常量命名

`类型名_常量名`（如 `CCARY_DEFAULT_CAPACITY`）。

### 7.5 头文件保护

```c
#ifndef 类型名_H
#define 类型名_H
/* ... */
#endif
```

## 第8章：高级主题

### 8.1 线程安全

- 文档标注线程安全性
- 必要时提供 `_locked` 版本或要求调用者加锁

### 8.2 不可变性

提供 `const` 版本的方法（getter），避免修改操作。

### 8.3 迭代器模式

提供 `类型名_iterator` 结构体，支持 `has_next`/`next`。

```c
typedef struct ccary_iterator ccary_iterator;
ccary_iterator *ccary_iterator_new(const ccary *ca);
void ccary_iterator_destroy(ccary_iterator *it);
int ccary_iterator_has_next(const ccary_iterator *it);
const char *ccary_iterator_next(ccary_iterator *it);
```

### 8.4 内存池（可选）

大对象可考虑支持自定义分配器，通过构造函数参数传入。

## 与现有代码的关系

- 保持现有 Doxygen 注释风格
- 保持中文文档要求
- 保持 `-Werror` 和测试要求
- `ccary` 等现有模块应逐步按此规范重构
