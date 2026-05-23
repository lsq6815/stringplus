# Changelog

All notable changes to this project will be documented in this file. See [commit-and-tag-version](https://github.com/absolute-version/commit-and-tag-version) for commit guidelines.

## [1.2.0](https://github.com/lsq6815/stringplus/compare/v1.1.0...v1.2.0) (2026-05-23)


### Features

* **object:** 实现 Object 基类（引用计数、虚函数、类型识别） ([c336f39](https://github.com/lsq6815/stringplus/commit/c336f39352005ebc97f504bba36d29dd5dcbc879))
* **object:** 添加 ObjClass 和 vtable 类型定义 ([5145945](https://github.com/lsq6815/stringplus/commit/5145945d6228be2d30e59ad1616bd003da45f2a5))
* **object:** 添加 ObjClass 和 vtable 类型定义 ([f17c2c0](https://github.com/lsq6815/stringplus/commit/f17c2c07ad3140d1027f4dc3ef97c092a0bd76be))
* **object:** 添加 Object 公共 API 头文件 ([bf77999](https://github.com/lsq6815/stringplus/commit/bf77999bffc10b48ba6d3558f32506dd5a174e2e))
* **test:** 引入 Unity 测试框架 v2.6.0 ([930c576](https://github.com/lsq6815/stringplus/commit/930c576861111f36e3cfd1a99f7aae77c1f4ae38))


### Bug Fixes

* 回退 test_stringplus 目标上不必要的 clang-tidy 禁用 ([4d5423a](https://github.com/lsq6815/stringplus/commit/4d5423a73cb94652a099a51f57b5668c4184be96))
* 回退测试目标上不必要的 clang-tidy 禁用 ([ffd0033](https://github.com/lsq6815/stringplus/commit/ffd003393479c5db0d4b20f98c951588133eced2))
* 禁用 test_stringplus 和 test_ccary 目标上对 unity.h 的 clang-tidy ([c82932c](https://github.com/lsq6815/stringplus/commit/c82932c16359939ad821f0ebd70ecccb9cb88d0e))
* **object:** 修复 obj_equals/obj_compare 不对称 isa 检查，移除未使用 include ([a424151](https://github.com/lsq6815/stringplus/commit/a424151d57c3927199b8f1a35e8c15b6e722b500))

## [1.1.0](https://github.com/lsq6815/stringplus/compare/v1.0.1...v1.1.0) (2026-05-23)


### Features

* 移除 main.c 并添加动态/静态库编译选项 ([0db9e60](https://github.com/lsq6815/stringplus/commit/0db9e60915e647342d2a816761fe856920eaf415))
* **ccary:** 实现不透明类型、指数扩容和迭代器 ([38505a3](https://github.com/lsq6815/stringplus/commit/38505a3bb136f5666d033e683e59f2576ce4ea5e))
* **ccary:** 重写头文件，引入不透明类型和新 API ([98c3436](https://github.com/lsq6815/stringplus/commit/98c3436ab02a1d20d31e0fbf3685c1063cf82c88))


### Bug Fixes

* **ccary:** iterator_destroy 处理 NULL 参数 ([58c6e28](https://github.com/lsq6815/stringplus/commit/58c6e28c6eb6bef1c17765357a5e2f2d5c9a00e5))

## 1.0.1 (2026-05-22)


### Bug Fixes

* **build:** 移除 docs 自定义目标中的 ALL 关键字 ([9741cfc](https://github.com/lsq6815/stringplus/commit/9741cfcd45088d171e1e53e1dc703c5497bbdc6e))
