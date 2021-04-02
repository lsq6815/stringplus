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

typedef void* ccary_foreach_func(void *);

/**
 * @brief construct and init a pccary
 *
 * @return pointer to new ccary
 */
pccary ccary_init();

/**
 * @brief destroy ccary pointed by @pcca, free all spaces
 *
 * @param pcca : pointer to ccary
 */
void ccary_destroy(pccary pcca);

/**
 * @brief foreach string in @pcca call @func
 * NOTE: calling free() in @func wiil caues memory leak!
 *
 * @param pcca pointer to ccary
 * @param func func to cope with string
 */
void ccary_foreach(pccary pcca, ccary_foreach_func func);

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
 * @brief append @str to @pcca
 *
 * @param pcca : pointer to ccary
 * @param str : c-style string
 */
void ccary_append(pccary pcca, const char *str);

/**
 * @brief display content of @pcca
 *
 * @param pcca pointer to ccary
 */
void ccary_display(pccary pcca);

#endif
