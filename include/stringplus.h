#ifndef STRINGPLUS_H
#define STRINGPLUS_H

/**
 * A wrapping for standard library string.h
 */

#include <string.h>
#include "ccary.h"

#define NOP (-1)

/**
 * @brief index the first appearance of @ch in @str
 *
 * @param str : c-style string
 * @param ch  : char to been index
 * @return NOP if @str hasn't @ch, otherwise the index
 */
size_t indexof(const char *str, int ch);

/**
 * @brief index first appearance of any char from @charset in the @str, 
 *
 * @param str     : c-style string
 * @param charset : a set of char to been index
 * @return NOP if @str hasn't match any char, otherwise the index
 */
size_t indexamong(const char *str, const char *charset);

/**
 * @brief split @str with separators from @delims
 *
 * @param str    : string to be split
 * @param delims : a set of delimiter
 * @return a ccary which carry the result of split
 */
pccary split(const char *str, const char *delims);

/**
 * @brief join strings from @strset with @jch to a single string
 *
 * @param strset : a set of string
 * @param jch    : char used to join strings
 * @return a single string
 */
char* join(pccary strset, char jch);

#endif
