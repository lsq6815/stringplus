#ifndef STRINGPLUS_H
#define STRINGPLUS_H

/**
 * 对标准库 string.h 的封装
 */

#include <string.h>
#include "ccary.h"

#define NOP ((size_t)-1)

/**
 * @brief 查找 @ch 在 @str 中第一次出现的位置
 *
 * @param str : C 风格字符串
 * @param ch  : 要查找的字符
 * @return 如果 @str 中没有 @ch 返回 NOP，否则返回下标
 */
size_t indexof(const char *str, int chr);

/**
 * @brief 查找 @charset 中任意字符在 @str 中第一次出现的位置
 *
 * @param str     : C 风格字符串
 * @param charset : 要查找的字符集合
 * @return 如果 @str 中没有任何匹配字符返回 NOP，否则返回下标
 */
size_t indexamong(const char *str, const char *charset);

/**
 * @brief 使用 @delims 中的分隔符分割 @str
 *
 * @param str    : 待分割的字符串
 * @param delims : 分隔符集合
 * @return 包含分割结果的 ccary
 */
pccary split(const char *str, const char *delims);

/**
 * @brief 使用 @jch 将 @strset 中的字符串连接成单个字符串
 *
 * @param strset : 字符串集合
 * @param jch    : 用于连接字符串的字符
 * @return 单个字符串
 */
char* join(pccary strset, char jch);

#endif
