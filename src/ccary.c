#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>  /* Supply integer type with exact bits */
#include <stddef.h>  /* Supply some definition like ptrdiff_t, size_t etc. */
#include <stdbool.h> /* Supply type _Bool, false and true */
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <math.h>

#include "../include/ccary.h"

void ccary_init(pccary pcca) {
    pcca->size = 0;
    pcca->data = NULL;
}

void ccary_destroy(pccary pcca) {
    size_t i;
    for (i = 0; i < pcca->size; i++) 
        free(pcca->data[i]);

    free(pcca->data);
}

void ccary_clean(pccary pcca) {
    size_t i;
    for (i = 0; i < pcca->size; i++) 
        free(pcca->data[i]);

    free(pcca->data);
    ccary_init(pcca);
}

size_t ccary_size(pccary pcca) {
    return pcca->size;
}
 
ccary ccary_append(ccary cca, const char *str) {
    char **arr = cca.data;
    size_t size = cca.size;

    /* increment the length of array */
    arr = realloc(arr, sizeof(char *) * (size + 1));

    /* allocate space for new string */ 
    arr[size] = malloc(strlen(str) + 1);
    strcpy(arr[size], str);

    ccary result = { size + 1, arr };
    return result;
}

void ccary_display(ccary cca) {
    size_t i;
    for (i = 0; i < cca.size; i++) {
        printf("|%s|\n", cca.data[i]);
    }
    printf("Total: %lu\n", ccary_size(&cca));
}
