#ifndef TEST_UTILS_H
#define TEST_UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define TEST_ASSERT(condition) \
    do { \
        if (!(condition)) { \
            fprintf(stderr, "ASSERTION FAILED at %s:%d: %s\n", __FILE__, __LINE__, #condition); \
            return 1; \
        } \
    } while (0)

#define TEST_ASSERT_EQUAL(expected, actual) \
    do { \
        if ((expected) != (actual)) { \
            fprintf(stderr, "ASSERTION FAILED at %s:%d: expected %ld but got %ld\n", \
                    __FILE__, __LINE__, (long)(expected), (long)(actual)); \
            return 1; \
        } \
    } while (0)

#define TEST_ASSERT_EQUAL_size_t(expected, actual) \
    do { \
        if ((expected) != (actual)) { \
            fprintf(stderr, "ASSERTION FAILED at %s:%d: expected %zu but got %zu\n", \
                    __FILE__, __LINE__, (size_t)(expected), (size_t)(actual)); \
            return 1; \
        } \
    } while (0)

#define TEST_ASSERT_EQUAL_STRING(expected, actual) \
    do { \
        if (strcmp((expected), (actual)) != 0) { \
            fprintf(stderr, "ASSERTION FAILED at %s:%d: expected \"%s\" but got \"%s\"\n", \
                    __FILE__, __LINE__, (expected), (actual)); \
            return 1; \
        } \
    } while (0)

#define TEST_ASSERT_NOT_NULL(ptr) \
    do { \
        if ((ptr) == NULL) { \
            fprintf(stderr, "ASSERTION FAILED at %s:%d: pointer is NULL\n", __FILE__, __LINE__); \
            return 1; \
        } \
    } while (0)

#define RUN_TEST(test_func) \
    do { \
        printf("  Running %s ... ", #test_func); \
        if (test_func() == 0) { \
            printf("PASSED\n"); \
            passed++; \
        } else { \
            printf("FAILED\n"); \
            failed++; \
        } \
    } while (0)

#define TEST_START() \
    int passed = 0, failed = 0; \
    printf("Running tests...\n");

#define TEST_END() \
    printf("\n%d passed, %d failed\n", passed, failed); \
    return failed > 0 ? 1 : 0;

#endif
