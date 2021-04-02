#include <string.h>

#include "../include/stringplus.h"

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
