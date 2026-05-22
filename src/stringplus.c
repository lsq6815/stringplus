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

pccary split(const char *str, const char *delims) {
    pccary result = ccary_init();
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

char* join(pccary strset, char jch) {
    size_t idx = 0;
    size_t len = 0;
    size_t cur = 0;
    char *result = NULL;

    for (idx = 0; idx < strset->size; idx++) {
        len += strlen(strset->data[idx]);
    }
    /* 需要添加的 jch 数量 */
    if (strset->size > 0) {
        len += strset->size - 1;
    }
    result = malloc(len + 1);

    for (idx = 0, cur = 0; idx < strset->size; idx++) {
        size_t slen = strlen(strset->data[idx]);
        memcpy(result + cur, strset->data[idx], slen + 1);
        cur += slen;
        result[cur] = jch;
        cur++;
    }
    result[len] = '\0';
    return result;
}
