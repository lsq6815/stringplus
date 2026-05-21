#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>  /* 提供固定位数的整数类型 */
#include <stddef.h>  /* 提供 ptrdiff_t、size_t 等定义 */
#include <stdbool.h> /* 提供 _Bool 类型以及 false 和 true */
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <math.h>

#include "../include/ccary.h"

pccary ccary_init() {
    pccary pcca = malloc(sizeof(ccary));
    pcca->size = 0;
    pcca->data = NULL;
    return pcca;
}

void ccary_destroy(pccary pcca) {
    size_t i;
    /* 释放字符串 */
    for (i = 0; i < pcca->size; i++) 
        free(pcca->data[i]);

    /* 释放数组 */
    free(pcca->data);
    /* 释放结构体 */
    free(pcca);
}

void ccary_foreach(pccary pcca, ccary_foreach_func func) {
    size_t i;
    for (i = 0; i < pcca->size; i++) {
        func(pcca->data[i]);
    }
}

void ccary_clean(pccary pcca) {
    size_t i;
    for (i = 0; i < pcca->size; i++) 
        free(pcca->data[i]);

    free(pcca->data);
    pcca->data = NULL;
    pcca->size = 0;
}

size_t ccary_size(pccary pcca) {
    return pcca->size;
}
 
void ccary_append(pccary pcca, const char *str) {
    /* 增加数组长度 */
    pcca->data = realloc(pcca->data, sizeof(char *) * (pcca->size + 1));

    /* 为新字符串分配空间 */ 
    pcca->data[pcca->size] = malloc(strlen(str) + 1);
    strcpy(pcca->data[pcca->size], str);

    pcca->size += 1;
}

static void* print_string(void *str) {
    printf("|%s|\n", (char *)str);
    return NULL;
}

void ccary_display(pccary pcca) {
    ccary_foreach(pcca, print_string);
    printf("Total: %lu\n", ccary_size(pcca));
}
