#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "../include/stringplus.h"
#include "../include/ccary.h"

size_t indexof(const char *str, int ch) {
    char *ptr = NULL;
    if ((ptr = strchr(str, ch)) != NULL)
        return ptr - str;

    return NOP;
}

size_t indexamong(const char *str, const char *charset) {
    size_t i = strcspn(str, charset);
    return i < strlen(str) ? i : NOP;
}

pccary split(const char *str, const char *delims) {
    pccary result = ccary_init();
    size_t i = 0, len;
    while ((len = indexamong(str + i, delims)) != NOP) {
        if (len > 0) {
            char *tmp = malloc(len + 1);
            strncpy(tmp, str + i, len);
            tmp[len] = '\0';
            ccary_append(result, tmp);
            free(tmp);
        } 
        i += len + 1;
    }

    len = strlen(str) - i;
    if (len > 0) {
        char *tmp = malloc(len + 1);
        strncpy(tmp, str + i, len);
        tmp[len] = '\0';
        ccary_append(result, tmp);
        free(tmp);
    } 

    return result;
}

char* join(pccary strset, char jch) {
    size_t i, len = 0, cur;
    char *result = NULL;

    for (i = 0; i < strset->size; i++) {
        len += strlen(strset->data[i]);
    }
    /* number of jch to be add */
    len += strset->size - 1;
    result = malloc(len + 1);

    for (i = 0, cur = 0; i < strset->size; i++) {
        strcpy(result + cur, strset->data[i]);
        cur += strlen(strset->data[i]);
        result[cur] = jch;
        cur++;
    }
    result[len] = '\0';
    return result;
}
