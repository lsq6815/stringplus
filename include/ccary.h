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
typedef void (*ccary_foreach_func)(const char *);

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
void ccary_display(const ccary *ca);

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
