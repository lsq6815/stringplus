# 测试框架迁移设计

**日期：** 2026-05-23
**状态：** 已确认

## 目标

移除自定义测试宏（`tests/test_utils.h`），引入 Unity 测试框架作为替代。

## 背景

当前项目使用 `tests/test_utils.h` 中的自定义宏实现简易测试：
- `TEST_ASSERT`, `TEST_ASSERT_EQUAL`, `TEST_ASSERT_EQUAL_size_t`, `TEST_ASSERT_EQUAL_STRING`, `TEST_ASSERT_NOT_NULL`, `TEST_ASSERT_NULL`, `TEST_ASSERT_EQUAL_INT`
- `RUN_TEST`, `RUN_TEST_NAMED`, `TEST_START`, `TEST_END`

Unity 是轻量级纯 C 单头文件+单源文件测试框架，断言宏命名与现有自定义宏高度一致，迁移成本极低。

## 方案选择：Unity + 纳入仓库

- Unity 文件（`unity.c` + `unity.h`）直接放入 `tests/unity/` 目录
- 不使用 CMake FetchContent，避免网络依赖

## 文件变更

| 操作 | 路径 | 说明 |
|------|------|------|
| 新增 | `tests/unity/unity.c` | Unity 框架实现 |
| 新增 | `tests/unity/unity.h` | Unity 框架头文件 |
| 删除 | `tests/test_utils.h` | 自定义测试宏，全部由 Unity 替代 |
| 修改 | `tests/test_stringplus.c` | 替换 include、断言宏、runner |
| 修改 | `tests/test_ccary.c` | 替换 include、断言宏、runner |
| 修改 | `tests/test_object.c` | 替换 include、断言宏、runner |
| 修改 | `tests/CMakeLists.txt` | 链接 unity.c 到所有测试目标 |

## 断言宏映射

自定义宏与 Unity 宏命名高度一致：

| 自定义宏 | Unity 等价 |
|----------|-----------|
| `TEST_ASSERT(cond)` | `TEST_ASSERT_TRUE(cond)` |
| `TEST_ASSERT_EQUAL(a,b)` | `TEST_ASSERT_EQUAL_INT64(a,b)` |
| `TEST_ASSERT_EQUAL_size_t(a,b)` | `TEST_ASSERT_EQUAL_size_t(a,b)` |
| `TEST_ASSERT_EQUAL_STRING(a,b)` | `TEST_ASSERT_EQUAL_STRING(a,b)` |
| `TEST_ASSERT_NOT_NULL(p)` | `TEST_ASSERT_NOT_NULL(p)` |
| `TEST_ASSERT_NULL(p)` | `TEST_ASSERT_NULL(p)` |
| `TEST_ASSERT_EQUAL_INT(a,b)` | `TEST_ASSERT_EQUAL_INT(a,b)` |

## Runner 模式变更

```c
/* 旧：test_utils.h */
TEST_START();
RUN_TEST_NAMED(test_func, "name");
TEST_END();

/* 新：Unity */
UNITY_BEGIN();
RUN_TEST(test_func);
UNITY_END();
```

不再需要手动维护测试名称数组，`RUN_TEST` 自动输出函数名。

## CMake 构建调整

- 新增 `unity.c` 编译为 CMake OBJECT 库，避免重复编译
- 移除对 `test_utils.h` 的引用

## 不变项

- 保持 3 个独立测试二进制文件不变
- 测试用例逻辑不变（仅替换 assert 宏和 runner 调用）
- CMake `add_test()` + `ctest` 运行方式不变
