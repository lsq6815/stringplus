#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "../include/stringplus.h"
#include "../include/ccary.h"

size_t indexof(const char *str, int chr) {
    char *ptr = strchr(str, chr);
    if (ptr != NULL) {
        return (size_t)(ptr - str);
    }

    return NOP;
}

size_t indexamong(const char *str, const char *charset) {
    size_t idx = strcspn(str, charset);
    return idx < strlen(str) ? idx : NOP;
}

ccary *split(const char *str, const char *delims) {
    ccary *result = ccary_create(0);
    size_t idx = 0;
    size_t len = 0;
    while ((len = indexamong(str + idx, delims)) != NOP) {
        if (len > 0) {
            char *tmp = malloc(len + 1);
            strncpy(tmp, str + idx, len);
            tmp[len] = '\0';
            ccary_append(result, tmp);
            free(tmp);
        }
        idx += len + 1;
    }

    len = strlen(str) - idx;
    if (len > 0) {
        char *tmp = malloc(len + 1);
        strncpy(tmp, str + idx, len);
        tmp[len] = '\0';
        ccary_append(result, tmp);
        free(tmp);
    }

    return result;
}

char* join(ccary *strset, char jch) {
    size_t idx = 0;
    size_t len = 0;
    size_t cur = 0;
    size_t size = ccary_get_size(strset);
    char *result = NULL;

    for (idx = 0; idx < size; idx++) {
        len += strlen(ccary_get_at(strset, idx));
    }
    /* 需要添加的 jch 数量 */
    if (size > 0) {
        len += size - 1;
    }
    result = malloc(len + 1);
    if (result == NULL) {
        return NULL;
    }

    for (idx = 0, cur = 0; idx < size; idx++) {
        size_t slen = strlen(ccary_get_at(strset, idx));
        memcpy(result + cur, ccary_get_at(strset, idx), slen + 1);
        cur += slen;
        result[cur] = jch;
        cur++;
    }
    if (size > 0) {
        result[len] = '\0';
    } else {
        result[0] = '\0';
    }
    return result;
}
