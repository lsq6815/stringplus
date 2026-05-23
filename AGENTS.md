# AGENTS.md

## 项目概述

小型 C 库，封装 `string.h` 并提供更高级的辅助函数，外加动态字符串数组（`ccary`）。使用 CMake 构建。

## 依赖

- CMake（必需）
- Node.js + npm（可选，用于 commitlint + husky）
- Doxygen（可选，用于文档生成）

安装 Node 依赖（如需要提交检查）：

```bash
npm install
```

## 构建与测试

```bash
cmake -S . -B build -G "MinGW Makefiles" -DCMAKE_C_COMPILER=gcc
cmake --build build
```

生成静态库 `libstringplus.a` 和可执行文件 `app`（演示 / 简易测试运行器）。

**运行测试：**

```bash
ctest --output-on-failure        # 运行所有测试
cd build && ./tests/test_stringplus   # 运行单个测试可执行文件
cd build && ./tests/test_ccary
```

> **注意：** 测试不使用 Unity 等框架，而是使用自定义的 `tests/test_utils.h` 宏（`TEST_ASSERT`, `RUN_TEST` 等）。

**生成文档：**

```bash
cmake --build . --target docs
```

生成的 HTML 位于 `build/docs/html/index.html`。

**手动运行 linter：**

```bash
cmake --build . --target lint
```

（需要安装 clang-tidy；未安装时构建会自动跳过。）

## 项目结构

| 路径 | 作用 |
|------|------|
| `src/stringplus.c`, `src/ccary.c` | 库实现 |
| `src/main.c` | 演示 / 简易测试运行器 |
| `include/stringplus.h`, `include/ccary.h` | 公共 API 头文件 |
| `tests/` | 单元测试（自定义测试框架） |

## 关键约定

- 源文件 include 风格：`#include "../include/stringplus.h"`（相对于 `src/` 的相对路径）。
- `ccary` 是动态字符串数组（`char**` + `size_t`）。调用者使用后必须调用 `ccary_destroy()`。
- `split()` 和 `join()` 返回堆分配的数据；调用者负责释放。
- `NOP` 宏（`((size_t)-1)`）用作"未找到"的哨兵值。
- `ccary_foreach()` 的回调中调用 `free()` 会导致内存泄漏。
## 注释风格

所有公共 API 使用 Doxygen 风格注释：

```c
/**
 * @brief 函数功能简述
 *
 * @param name : 参数说明
 * @return 返回值说明
 */
```

## 提交规范

采用 Conventional Commits，格式：`<type>(<scope>): <description>`

允许的 `type`：`feat`, `fix`, `docs`, `style`, `refactor`, `perf`, `test`, `chore`, `ci`, `build`, `revert`

约束：
- **提交信息使用中文书写**
- **文档使用中文书写**

相关配置：
- `commitlint.config.js` — commitlint 规则
- `.husky/commit-msg` — Git hook 脚本

## 版本发布

本项目使用 [commit-and-tag-version](https://github.com/absolute-version/commit-and-tag-version) 自动管理版本号、生成 CHANGELOG 和 git tag。

**发布命令：**

```bash
npm run release                # 自动推断版本级别（patch/minor/major）
npm run release -- --release-as minor   # 手动指定版本级别
npm run release -- --release-as major
npm run release -- --release-as 1.2.3   # 指定具体版本号
```

**发布流程：**

1. 确保所有变更已提交且符合 Conventional Commits 规范
2. 运行 `npm run release`（或带参数的变体）
3. 工具将自动：
   - 根据提交历史推断版本级别
   - 更新 `package.json` 中的版本号
   - 更新 `CHANGELOG.md`
   - 创建版本提交并打 git tag
4. 执行 `git push --follow-tags origin main` 推送 tag 到远程

**注意事项：**

- 发布前必须确保工作区干净（无未提交变更）
- 所有提交必须符合 Conventional Commits 规范，否则版本推断可能不正确
- tag 推送不会自动触发 CI/CD，需手动推送

## 构建约束

- CMake 生成目录**必须**使用 `build/`，不得使用其他目录。

## 代码质量约束

- 编译器警告全部视为错误（`-Werror`）。
- 每次修改源代码后，**必须**执行构建（`cmake --build .`），确保无警告。
- 每次修改源代码后，**必须**执行测试（`ctest --output-on-failure`），确保全部通过。
- 提交代码前，**必须**通过上述两项检查。

## C 语言面向对象编码规范

所有新增的结构体封装必须遵循本规范，现有代码（如 `ccary`）应逐步按此规范重构。

### 类型系统与内存管理

#### 不透明类型（Opaque Type）

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

#### 禁止值类型别名

只保留指针类型别名，禁止定义值类型别名。值拷贝会导致双重释放和未定义行为。

```c
/* 正确 */
typedef struct object object;

/* 错误 */
typedef struct object obj_t;  /* 值拷贝不安全 */
```

#### 所有权模型

- 构造函数返回的指针由调用者拥有
- `*_destroy()` 必须释放对象及其管理的所有资源
- 禁止浅拷贝（除非显式设计为引用语义）

### 构造函数与析构函数模式

#### 命名规范

- **构造函数**：`类型名_create()`、`类型名_init()` 或 `类型名_new()`
- **析构函数**：`类型名_destroy()` 或 `类型名_free()`

#### 构造函数要求

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

#### 析构函数要求

- 释放对象及其管理的所有资源
- 处理 `NULL` 参数（静默返回）

```c
void object_destroy(object *obj) {
    if (obj == NULL) return;
    free(obj->data);
    free(obj);
}
```

### 封装与访问控制

#### 信息隐藏

- 所有字段必须隐藏在 `.c` 文件中
- 用户只能通过 getter/setter 访问内部状态

#### 访问器命名

- **Getter**：`类型名_get_字段名()`，返回 `const` 指针或值
- **Setter**：`类型名_set_字段名()`，深拷贝输入值

```c
const char *object_get_data(const object *obj);
int object_set_data(object *obj, const char *data);
```

#### 禁止直接暴露可变成员

如必须暴露数组，提供 `类型名_get_count()` 和 `类型名_get_at()` 代替直接访问。

### 方法组织与命名空间

#### 函数前缀

所有公共 API 必须以 `类型名_` 为前缀，模拟命名空间。

```c
/* 正确 */
void ccary_append(ccary *ca, const char *str);

/* 错误 */
void append_string(ccary *ca, const char *str);  /* 缺少前缀 */
```

#### 私有函数

内部辅助函数使用 `static`，不加前缀。

#### 函数指针表（可选）

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

### 继承与多态模拟

#### 结构体嵌套

子类型在第一个字段嵌入父类型，确保地址兼容。

```c
struct child {
    struct parent base;  /* 必须是第一个字段 */
    int extra_field;
};

/* 允许向上转换 */
parent *p = (parent *)&child_instance;
```

#### 禁止直接访问父类字段

通过父类提供的 getter/setter 访问，保持封装。

### 错误处理策略

#### 返回码

方法返回 `int`，`0` 表示成功，`-1` 表示失败。

#### 错误信息查询

提供 `类型名_get_error()` 或全局错误码获取详细错误信息。

#### 资源安全

失败时必须保持对象处于一致状态，已分配的资源已释放。

#### NULL 检查

所有公共 API 必须在入口处检查参数是否为 `NULL`。

```c
int object_set_data(object *obj, const char *data) {
    if (obj == NULL || data == NULL) return -1;
    /* ... */
}
```

### 命名与代码风格

#### 文件命名

- 实现文件：`类型名.c`
- 头文件：`类型名.h`

#### 类型命名

小写 + 下划线（如 `string_builder`、`dynamic_array`）。

#### 函数命名

`类型名_动词_名词`（如 `ccary_append_string`、`string_builder_clear`）。

#### 常量命名

`类型名_常量名`（如 `CCARY_DEFAULT_CAPACITY`）。

#### 头文件保护

```c
#ifndef 类型名_H
#define 类型名_H
/* ... */
#endif
```

### 高级主题

#### 线程安全

- 文档标注线程安全性
- 必要时提供 `_locked` 版本或要求调用者加锁

#### 不可变性

提供 `const` 版本的方法（getter），避免修改操作。

#### 迭代器模式

提供 `类型名_iterator` 结构体，支持 `has_next`/`next`。

```c
typedef struct ccary_iterator ccary_iterator;
ccary_iterator *ccary_iterator_new(const ccary *ca);
void ccary_iterator_destroy(ccary_iterator *it);
int ccary_iterator_has_next(const ccary_iterator *it);
const char *ccary_iterator_next(ccary_iterator *it);
```

#### 内存池（可选）

大对象可考虑支持自定义分配器，通过构造函数参数传入。
