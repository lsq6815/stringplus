#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/stringplus.h"
#include "../include/ccary.h"

int main(int argc, char** argv) {
    (void)argc;
    (void)argv;
    const char *hello_world = "Hello, world";
    size_t idx = 0;
    idx = indexamong(hello_world, ",;");
    if (idx != NOP) {
        printf("Found '%c' in |%s| at %zu\n",
                hello_world[idx], hello_world, idx);
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

