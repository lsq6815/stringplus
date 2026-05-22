#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/ccary.h"

pccary ccary_init(void) {
    pccary pcca = malloc(sizeof(ccary));
    pcca->size = 0;
    pcca->data = NULL;
    return pcca;
}

void ccary_destroy(pccary pcca) {
    size_t idx = 0;
    /* 释放字符串 */
    for (idx = 0; idx < pcca->size; idx++) {
        free(pcca->data[idx]);
    }

    /* 释放数组 */
    free((void *)pcca->data);
    /* 释放结构体 */
    free(pcca);
}

void ccary_foreach(pccary pcca, ccary_foreach_func func) {
    size_t idx = 0;
    for (idx = 0; idx < pcca->size; idx++) {
        func(pcca->data[idx]);
    }
}

void ccary_clean(pccary pcca) {
    size_t idx = 0;
    for (idx = 0; idx < pcca->size; idx++) {
        free(pcca->data[idx]);
    }

    free((void *)pcca->data);
    pcca->data = NULL;
    pcca->size = 0;
}

size_t ccary_size(pccary pcca) {
    return pcca->size;
}

void ccary_append(pccary pcca, const char *str) {
    /* 增加数组长度 */
    char **tmp = (char **)realloc((void *)pcca->data, sizeof(char *) * (pcca->size + 1));
    if (tmp == NULL) {
        return;
    }
    pcca->data = tmp;

    /* 为新字符串分配空间 */
    pcca->data[pcca->size] = malloc(strlen(str) + 1);
    memcpy(pcca->data[pcca->size], str, strlen(str) + 1);

    pcca->size += 1;
}

static void print_string(void *str) {
    printf("|%s|\n", (char *)str);
}

void ccary_display(pccary pcca) {
    ccary_foreach(pcca, print_string);
    printf("Total: %zu\n", ccary_size(pcca));
}
