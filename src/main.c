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
    const char *hello_world = "Hello, world";
    size_t i;
    if ((i = indexamong(hello_world, ",;")) != NOP) {
        printf("Found '%c' in |%s| at %lu\n", 
                hello_world[i], hello_world, i);
    } 

    const char* rawstr = "8208180119-lsq_infosec";
    printf("Split |%s| into: \n", rawstr);

    pccary pcca = split(rawstr, "-_");
    ccary_display(pcca);

    char* joinstr = join(pcca, '+');
    printf("And join as |%s|\n", joinstr);

    free(joinstr);
    ccary_destroy(pcca);
    return 0;
}

