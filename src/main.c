#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>  /* Supply integer type with exact bits */
#include <stddef.h>  /* Supply some definition like ptrdiff_t, size_t etc. */
#include <stdbool.h> /* Supply type _Bool, false and true */
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <math.h>

#include "../include/stringplus.h"
#include "../include/ccary.h"

int main(int argc, char** argv) {
    const char *str1 = "Hello, world";
    size_t i;
    if ((i = indexamong(str1, ", ")) != NOP) {
        printf("Found %c in %s at %lu\n", 
                str1[i], str1, i);
    } 

    ccary cca;
    ccary_init(&cca);

    cca = ccary_append(cca, "Hello");
    ccary_display(cca);

    ccary_destroy(&cca);

    return 0;
}

