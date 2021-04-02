#ifndef CCARY_H
#define CCARY_H

#include <string.h>

/**
 * A warping of c-style string array
 */

typedef struct _ccary {
    size_t size;
    char** data;
} ccary, *pccary;

/**
 * @brief init ccary pointed by @pcca, set size to 0 and data to NULL
 *
 * @param pcca : pointer to ccary
 */
void ccary_init(pccary pcca);

/**
 * @brief destroy ccary pointed by @pcca, free all spaces
 *
 * @param pcca : pointer to ccary
 */
void ccary_destroy(pccary pcca);

/**
 * @brief clean all spaces pointed by @pcca, and set size to 0
 *
 * @param pcca pointed by @pcca
 */
void ccary_clean(pccary pcca);

/**
 * @brief get size of ccary
 *
 * @param pcca : pointer to ccary
 * @return size of ccary
 */
size_t ccary_size(pccary pcca);

/**
 * @brief append @str to @cca
 *
 * @param cca : ccary
 * @param str : c-style string
 * @return new ccary
 */
ccary ccary_append(ccary cca, const char *str);

/**
 * @brief display content of @cca
 *
 * @param cca ccary
 */
void ccary_display(ccary cca);

#endif
