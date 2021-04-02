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

pccary ccary_init() {
    pccary pcca = malloc(sizeof(ccary));
    pcca->size = 0;
    pcca->data = NULL;
    return pcca;
}

void ccary_destroy(pccary pcca) {
    size_t i;
    /* free strings */
    for (i = 0; i < pcca->size; i++) 
        free(pcca->data[i]);

    /* free array */
    free(pcca->data);
    /* free the strut */
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
    /* increment the length of array */
    pcca->data = realloc(pcca->data, sizeof(char *) * (pcca->size + 1));

    /* allocate space for new string */ 
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
