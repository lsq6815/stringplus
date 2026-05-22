#ifndef CCARY_H
#define CCARY_H

#include <string.h>

/**
 * C 风格字符串数组的封装
 */

typedef struct ccary {
    size_t size;
    char** data;
} ccary, *pccary;

typedef void (*ccary_foreach_func)(void *);

/**
 * @brief 构造并初始化一个 pccary
 *
 * @return 指向新 ccary 的指针
 */
pccary ccary_init(void);

/**
 * @brief 销毁 pcca 指向的 ccary，释放所有空间
 *
 * @param pcca : 指向 ccary 的指针
 */
void ccary_destroy(pccary pcca);

/**
 * @brief 遍历 pcca 中的每个字符串并调用 func
 * 注意：在 func 中调用 free() 会导致内存泄漏！
 *
 * @param pcca : 指向 ccary 的指针
 * @param func : 处理字符串的函数
 */
void ccary_foreach(pccary pcca, ccary_foreach_func func);

/**
 * @brief 清理 pcca 指向的所有空间，并将 size 置为 0
 *
 * @param pcca : 指向 ccary 的指针
 */
void ccary_clean(pccary pcca);

/**
 * @brief 获取 ccary 的大小
 *
 * @param pcca : 指向 ccary 的指针
 * @return ccary 的大小
 */
size_t ccary_size(pccary pcca);

/**
 * @brief 将 str 追加到 pcca
 *
 * @param pcca : 指向 ccary 的指针
 * @param str  : C 风格字符串
 */
void ccary_append(pccary pcca, const char *str);

/**
 * @brief 显示 pcca 的内容
 *
 * @param pcca : 指向 ccary 的指针
 */
void ccary_display(pccary pcca);

#endif
