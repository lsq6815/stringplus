# Object OOP 基础设施设计文档

## 日期：2026-05-23

## 1. 背景与动机

项目现有的 C OOP 编码规范（AGENTS.md）已定义了不透明类型、构造函数/析构函数、访问器等模式（如 `ccary` 的实现），但缺少一个统一的**基类**和**运行时基础设施**。各类型独立实现内存管理，无法共享引用计数、虚函数多态、运行时类型识别等通用能力。

本设计为项目引入一个轻量级的 Object 基类及配套基础设施，为未来所有新类型提供统一的 OOP 能力，同时保持与现有代码的兼容。

## 2. 设计目标

- 提供统一的引用计数内存管理，避免内存泄漏和悬空指针
- 支持虚函数多态，所有对象方法通过 vtable 间接调用
- 支持单继承和运行时类型识别（`isKindOfClass`）
- 保持轻量，不引入完整的反射、元类或消息转发机制
- 遵循项目已有的 C OOP 编码规范
- 作为独立基础设施，不强制现有代码迁移

## 3. 核心类型系统

### 3.1 Object 内存布局

每个 Object 实例的内存布局如下：

```
+------------------+
| isa (ObjClass *) |  <- 指向类型信息结构
| ref_count        |  <- 引用计数（size_t）
| ... 子类字段 ...  |
+------------------+
```

```c
/* object.h - 公共 API */
typedef struct object object;
```

```c
/* object.c - 完整定义 */
struct object {
    struct obj_class *isa;
    size_t ref_count;
};
```

### 3.2 ObjClass（类型信息结构）

`ObjClass` 保存类型的元信息和虚函数表：

```c
/* obj_class.h */
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
```

### 3.3 单继承模型

子类通过**结构体嵌套**实现继承：

```c
struct child {
    object base;        /* 必须是第一个字段 */
    int extra_field;
};
```

子类 `child` 的 `isa` 指向自己的 `obj_class`，`obj_class.parent` 指向父类的 `obj_class`。这允许向上转型和运行时类型识别。

## 4. 内存管理与引用计数

### 4.1 所有权规则

- `*_create()` 返回的对象引用计数为 1，由调用者拥有
- `obj_retain()` 增加引用计数
- `obj_release()` 减少引用计数；归零时自动调用 `dealloc`

### 4.2 引用计数 API

```c
void obj_retain(object *obj);
void obj_release(object *obj);
```

### 4.3 析构链

`dealloc` 方法的责任：
1. 释放子类特有的资源
2. 调用父类的 `dealloc`（通过 `obj->isa->parent->vtable.dealloc(obj)`）

这模拟了 C++ 的析构函数链，确保基类资源也被正确释放。

## 5. 构造函数与析构函数模式

### 5.1 基类构造

```c
object *object_create(void);
```

实现逻辑：
1. 分配 `sizeof(object)` 内存
2. 设置 `isa` 指向全局的 `Object` 类
3. 设置 `ref_count = 1`
4. 返回对象指针

### 5.2 子类构造模式

```c
child *child_create(void) {
    child *c = malloc(sizeof(child));
    if (c == NULL) return NULL;
    
    /* 初始化基类 */
    c->base.isa = &ChildClass;
    c->base.ref_count = 1;
    
    /* 初始化子类字段 */
    c->extra_field = 0;
    
    return c;
}
```

### 5.3 析构函数要求

- 处理 `NULL` 参数（静默返回）
- 释放对象及其管理的所有资源
- 通过 vtable 调用，确保调用正确的析构链

## 6. 虚函数调用机制

### 6.1 调用方式

运行时基础设施分为两类调用：

**直接函数调用（isa / ref_count 相关）：**
与对象生命周期和类型识别相关的操作直接以函数提供，不经过 vtable：

```c
void obj_retain(object *obj);
void obj_release(object *obj);
int obj_is_kind_of_class(const object *obj, const obj_class *cls);
int obj_is_member_of_class(const object *obj, const obj_class *cls);
const char *obj_class_name(const object *obj);
const obj_class *obj_get_class(const object *obj);
```

**vtable 调用（对象行为相关）：**
描述、比较、哈希等行为通过 `isa->vtable` 间接调用：

```c
const char *obj_describe(object *obj);   /* 内部调用 vtable->describe */
int obj_equals(object *a, object *b);     /* 内部调用 vtable->equals */
int obj_compare(object *a, object *b);    /* 内部调用 vtable->compare */
size_t obj_hash(object *obj);             /* 内部调用 vtable->hash */
```

### 6.2 虚函数表填充

子类定义自己的 `obj_class` 时，复制父类的 vtable 并覆盖需要自定义的方法：

```c
static const obj_vtable child_vtable = {
    .dealloc = child_dealloc,      /* 覆盖 */
    .describe = child_describe,    /* 覆盖 */
    .equals = object_equals,       /* 继承父类 */
    .compare = object_compare,     /* 继承父类 */
    .hash = object_hash            /* 继承父类 */
};

const obj_class ChildClass = {
    .name = "Child",
    .parent = &ObjectClass,
    .vtable = child_vtable
};
```

## 7. 运行时类型识别

基于 `obj_class` 的 `parent` 指针链实现：

```c
int obj_is_kind_of_class(const object *obj, const obj_class *cls);
int obj_is_member_of_class(const object *obj, const obj_class *cls);
const char *obj_class_name(const object *obj);
const obj_class *obj_get_class(const object *obj);
```

- `obj_is_kind_of_class`：遍历 `obj->isa` 的 `parent` 链，判断是否等于 `cls`
- `obj_is_member_of_class`：仅比较 `obj->isa == cls`
- 不提供反射（如获取 ivar 列表、方法列表），保持轻量

## 8. 文件结构

```
include/
  object.h      /* Object 基类公共 API */
  obj_class.h   /* ObjClass、vtable 定义（子类定义时需要）*/
src/
  object.c      /* Object 基类实现 */
tests/
  test_object.c /* 引用计数、类型识别、虚函数调用测试 */
```

## 9. API 设计

### 9.1 object.h（公共 API）

```c
#ifndef OBJECT_H
#define OBJECT_H

#include "obj_class.h"

object *object_create(void);

void obj_retain(object *obj);
void obj_release(object *obj);

const char *obj_describe(object *obj);
int obj_equals(object *a, object *b);
int obj_compare(object *a, object *b);
size_t obj_hash(object *obj);

int obj_is_kind_of_class(const object *obj, const obj_class *cls);
int obj_is_member_of_class(const object *obj, const obj_class *cls);
const char *obj_class_name(const object *obj);
const obj_class *obj_get_class(const object *obj);

#endif
```

### 9.2 obj_class.h（类型定义）

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

## 10. 兼容性说明

- **现有代码**：`ccary`、`stringplus` 等现有类型不受影响，可逐步选择是否继承 `object`
- **新类型**：未来所有新类型可选择继承 `object` 以获得统一的内存管理和多态能力
- **不透明类型**：`object` 保持不透明，内部结构仅在 `object.c` 中定义

## 11. 代码风格

- 函数命名：`obj_动词`（如 `obj_retain`、`obj_describe`）或 `object_动词`（构造函数）
- 类型命名：小写 + 下划线（`obj_class`、`obj_vtable`）
- 常量命名：`ObjectClass`（全局类对象）
- 头文件保护：`OBJECT_H`、`OBJ_CLASS_H`
- 注释：Doxygen 风格
- 私有函数：使用 `static`，不加前缀
