#ifndef OBJ_CLASS_H
#define OBJ_CLASS_H

#include <stddef.h>

typedef struct object object;
typedef struct obj_class obj_class;

/**
 * @brief 虚函数表（vtable），定义对象的基本行为
 *
 * 每个 obj_class 实例包含一个 vtable，用于实现面向对象的多态行为。
 */
typedef struct obj_vtable {
    /** @brief 释放对象资源 */
    void (*dealloc)(object *obj);
    /** @brief 返回对象的描述字符串 */
    const char *(*describe)(const object *obj);
    /** @brief 判断两个对象是否相等 */
    int (*equals)(const object *a, const object *b);
    /** @brief 比较两个对象的大小 */
    int (*compare)(const object *a, const object *b);
    /** @brief 计算对象的哈希值 */
    size_t (*hash)(const object *obj);
} obj_vtable;

/**
 * @brief 类描述符，定义对象的类型信息
 *
 * @note 本结构体有意在头文件中暴露定义，以便子类可以进行静态类描述符初始化
 *       （例如创建 static const obj_class 实例）。这是不透明类型规则的有意例外。
 */
struct obj_class {
    /** @brief 类名 */
    const char *name;
    /** @brief 父类指针，NULL 表示无父类 */
    const obj_class *parent;
    /** @brief 虚函数表 */
    const obj_vtable vtable;
};

/**
 * @brief 全局基类（Object）的类描述符，供所有子类引用
 */
extern const obj_class obj_class_object;

#endif
