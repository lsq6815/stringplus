# ccary C OOP 重构设计文档

## 日期：2026-05-23

## 1. 背景与动机

当前 `ccary` 实现存在以下与 AGENTS.md C 面向对象编码规范不符的问题：

- 头文件中直接暴露结构体定义，违反不透明类型原则
- 同时定义了值类型别名 `ccary` 和指针类型别名 `pccary`，允许值拷贝
- 缺少错误码返回机制（`ccary_append` 返回 `void`）
- 每次 `append` 都精确分配内存，性能较差
- 测试代码直接访问内部字段 `pcca->data[0]`，破坏封装
- 缺少迭代器模式（规范中建议提供）

## 2. 设计目标

- 完全遵循 C OOP 规范：不透明类型、命名空间前缀、构造函数/析构函数模式
- 引入 `capacity` 字段实现指数扩容，提升性能
- 所有可能失败的操作返回 `int` 错误码
- 提供迭代器 API
- 重构测试代码，仅通过公共 API 验证

## 3. 类型系统

### 3.1 不透明类型

头文件仅暴露不完整声明：

```c
/* ccary.h */
typedef struct ccary ccary;
```

完整结构体定义仅在实现文件中：

```c
/* ccary.c */
struct ccary {
    size_t size;
    size_t capacity;
    char **data;
};
```

### 3.2 移除值类型别名

- **移除** `typedef struct ccary { ... } ccary, *pccary;`
- **移除** `pccary` 别名
- 统一使用 `ccary *`

## 4. API 设计

### 4.1 构造函数与析构函数

| 函数 | 签名 | 说明 |
|------|------|------|
| 构造 | `ccary *ccary_create(size_t initial_capacity);` | `initial_capacity` 为 0 时使用默认值（16） |
| 析构 | `void ccary_destroy(ccary *ca);` | 处理 NULL 参数，释放所有资源 |

### 4.2 核心操作（返回 int 错误码）

| 函数 | 签名 | 说明 |
|------|------|------|
| 追加 | `int ccary_append(ccary *ca, const char *str);` | 深拷贝字符串；0=成功，-1=失败 |
| 清空 | `int ccary_clean(ccary *ca);` | 释放所有元素，保留容量；0=成功，-1=失败 |
| 遍历 | `int ccary_foreach(ccary *ca, ccary_foreach_func func);` | 对每个元素调用回调；0=成功，-1=失败 |

### 4.3 访问器（Getter）

| 函数 | 签名 | 说明 |
|------|------|------|
| 大小 | `size_t ccary_get_size(const ccary *ca);` | 返回元素数量 |
| 容量 | `size_t ccary_get_capacity(const ccary *ca);` | 返回当前容量 |
| 获取元素 | `const char *ccary_get_at(const ccary *ca, size_t index);` | 越界返回 NULL |

### 4.4 迭代器

```c
typedef struct ccary_iterator ccary_iterator;

typedef struct ccary_iterator ccary_iterator;
ccary_iterator *ccary_iterator_create(const ccary *ca);
void ccary_iterator_destroy(ccary_iterator *it);
int ccary_iterator_has_next(const ccary_iterator *it);
const char *ccary_iterator_next(ccary_iterator *it);
```

## 5. 扩容策略

- **默认初始容量**：`CCARY_DEFAULT_CAPACITY = 16`
- **扩容公式**：
  ```c
  new_capacity = (capacity == 0) ? 1 : capacity * 2;
  ```
- **行为**：`ccary_append` 检测到 `size == capacity` 时触发扩容；`ccary_clean` 保留容量

## 6. 错误处理

- 所有可能分配内存或操作数组的公共 API 返回 `int`：
  - `0`：成功
  - `-1`：失败（内存分配失败、NULL 参数等）
- 构造函数返回 `NULL` 表示失败
- 析构函数接受 `NULL` 参数，静默返回

## 7. 内存管理

- **所有权**：`ccary_create` 返回的指针由调用者拥有
- **深拷贝**：`ccary_append` 对输入字符串进行深拷贝
- **清理**：`ccary_destroy` 释放对象、数组及所有字符串

## 8. 测试重构

### 8.1 修改点

| 原测试代码 | 重构后 |
|-----------|--------|
| `TEST_ASSERT(pcca->data == NULL);` | 移除（无法访问内部字段） |
| `TEST_ASSERT_EQUAL_STRING("Hello", pcca->data[0]);` | `TEST_ASSERT_EQUAL_STRING("Hello", ccary_get_at(pcca, 0));` |
| `TEST_ASSERT(pcca->data == NULL);`（clean 后） | 改用 `ccary_get_size(pcca) == 0` 和 `ccary_get_capacity(pcca)` 验证 |

### 8.2 新增测试

- `test_ccary_iterator`：验证迭代器创建、遍历、越界行为
- `test_ccary_get_at_out_of_bounds`：验证越界返回 NULL
- `test_ccary_capacity_growth`：验证容量增长

## 9. 文件变更

| 文件 | 变更类型 |
|------|----------|
| `include/ccary.h` | 重写：不透明类型、新 API |
| `src/ccary.c` | 重写：完整结构体、新实现 |
| `tests/test_ccary.c` | 重构：使用公共 API、新增测试 |

## 10. 兼容性说明

- **破坏性变更**：`pccary` 别名被移除，所有使用 `pccary` 的代码需要改为 `ccary *`
- **main.c**：需要检查并修复 `pccary` 的使用
- **头文件保护**：保持 `#ifndef CCARY_H` 不变

## 11. 代码风格

- 函数命名：`ccary_动词_名词`
- 常量命名：`CCARY_DEFAULT_CAPACITY`
- 注释：Doxygen 风格
- 私有函数：使用 `static`，不加前缀
