#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/ccary.h"

enum { CCARY_DEFAULT_CAPACITY = 16 };

struct ccary {
    size_t size;
    size_t capacity;
    char **data;
};

struct ccary_iterator {
    const ccary *ca;
    size_t current;
};

ccary *ccary_create(size_t initial_capacity) {
    ccary *ca = malloc(sizeof(ccary));
    if (ca == NULL) {
        return NULL;
    }

    ca->size = 0;
    ca->capacity = (initial_capacity == 0) ? CCARY_DEFAULT_CAPACITY : initial_capacity;
    ca->data = (char **)malloc(sizeof(char *) * ca->capacity);
    if (ca->data == NULL) {
        free(ca);
        return NULL;
    }

    return ca;
}

void ccary_destroy(ccary *ca) {
    size_t idx = 0;
    if (ca == NULL) {
        return;
    }

    for (idx = 0; idx < ca->size; idx++) {
        free(ca->data[idx]);
    }

    free((void *)ca->data);
    free(ca);
}

int ccary_foreach(const ccary *ca, ccary_foreach_func func) {
    size_t idx = 0;
    if (ca == NULL || func == NULL) {
        return -1;
    }

    for (idx = 0; idx < ca->size; idx++) {
        func(ca->data[idx]);
    }

    return 0;
}

int ccary_clean(ccary *ca) {
    size_t idx = 0;
    if (ca == NULL) {
        return -1;
    }

    for (idx = 0; idx < ca->size; idx++) {
        free(ca->data[idx]);
    }

    ca->size = 0;
    return 0;
}

size_t ccary_get_size(const ccary *ca) {
    if (ca == NULL) {
        return 0;
    }
    return ca->size;
}

size_t ccary_get_capacity(const ccary *ca) {
    if (ca == NULL) {
        return 0;
    }
    return ca->capacity;
}

const char *ccary_get_at(const ccary *ca, size_t index) {
    if (ca == NULL || index >= ca->size) {
        return NULL;
    }
    return ca->data[index];
}

int ccary_append(ccary *ca, const char *str) {
    char **tmp = NULL;
    size_t new_capacity = 0;

    if (ca == NULL || str == NULL) {
        return -1;
    }

    if (ca->size >= ca->capacity) {
        new_capacity = (ca->capacity == 0) ? 1 : ca->capacity * 2;
        tmp = (char **)realloc((void *)ca->data, sizeof(char *) * new_capacity);
        if (tmp == NULL) {
            return -1;
        }
        ca->data = tmp;
        ca->capacity = new_capacity;
    }

    ca->data[ca->size] = malloc(strlen(str) + 1);
    if (ca->data[ca->size] == NULL) {
        return -1;
    }

    memcpy(ca->data[ca->size], str, strlen(str) + 1);
    ca->size += 1;
    return 0;
}

static void print_string(const char *str) {
    printf("|%s|\n", str);
}

void ccary_display(const ccary *ca) {
    if (ca == NULL) {
        return;
    }
    ccary_foreach(ca, print_string);
    printf("Total: %zu\n", ccary_get_size(ca));
}

ccary_iterator *ccary_iterator_create(const ccary *ca) {
    ccary_iterator *it = NULL;
    if (ca == NULL) {
        return NULL;
    }

    it = malloc(sizeof(ccary_iterator));
    if (it == NULL) {
        return NULL;
    }

    it->ca = ca;
    it->current = 0;
    return it;
}

void ccary_iterator_destroy(ccary_iterator *it) {
    free(it);
}

int ccary_iterator_has_next(const ccary_iterator *it) {
    if (it == NULL || it->ca == NULL) {
        return -1;
    }
    return it->current < it->ca->size ? 1 : 0;
}

const char *ccary_iterator_next(ccary_iterator *it) {
    if (it == NULL || it->ca == NULL || it->current >= it->ca->size) {
        return NULL;
    }
    return it->ca->data[it->current++];
}
