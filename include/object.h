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
