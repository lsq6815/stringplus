#ifndef STRINGPLUS_H
#define STRINGPLUS_H

/**
 * A wrapping for standard library string.h
 */

#include <string.h>
#include "ccary.h"

#define NOP (-1)

/**
 * @brief index the first apperence of @ch in @str
 *
 * @param str string
 * @param ch char
 * @return NOP if @str hasn't @ch, otherwise the index
 */
size_t indexof(const char *str, int ch);

/**
 * @brief index first apperence of any @ch from charset in the @str, 
 *
 * @param str string
 * @param charset charset
 * @return NOP if @str hasn't match any char, otherwise the index
 */
size_t indexamong(const char *str, const char *charset);

/**
 * @brief split @str with separators from @delims
 *
 * @param str string to be split
 * @param delims a set of delimiters
 * @return a ccary carry a array of string
 */
ccary split(const char *str, const char *delims);

/**
 * @brief join strings from @strset with @jch to a single string
 *
 * @param strset a set of strings
 * @param jch char to join strings
 * @return a single string
 */
char* join(ccary strset, char jch);

#endif
