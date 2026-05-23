#include "test_utils.h"
#include "ccary.h"

static int test_ccary_create_destroy(void) {
    ccary *ca = ccary_create(0);
    TEST_ASSERT_NOT_NULL(ca);
    TEST_ASSERT_EQUAL_size_t(0, ccary_get_size(ca));
    TEST_ASSERT_EQUAL_size_t(16, ccary_get_capacity(ca));
    ccary_destroy(ca);
    return 0;
}

static int test_ccary_create_with_capacity(void) {
    ccary *ca = ccary_create(8); /* NOLINT(readability-magic-numbers) */
    TEST_ASSERT_NOT_NULL(ca);
    TEST_ASSERT_EQUAL_size_t(0, ccary_get_size(ca));
    TEST_ASSERT_EQUAL_size_t(8, ccary_get_capacity(ca));
    ccary_destroy(ca);
    return 0;
}

static int test_ccary_append(void) {
    ccary *ca = ccary_create(0);
    TEST_ASSERT_EQUAL_INT(0, ccary_append(ca, "Hello"));
    TEST_ASSERT_EQUAL_size_t(1, ccary_get_size(ca));
    TEST_ASSERT_EQUAL_STRING("Hello", ccary_get_at(ca, 0));
    TEST_ASSERT_EQUAL_INT(0, ccary_append(ca, "world"));
    TEST_ASSERT_EQUAL_size_t(2, ccary_get_size(ca));
    TEST_ASSERT_EQUAL_STRING("world", ccary_get_at(ca, 1));
    ccary_destroy(ca);
    return 0;
}

static int test_ccary_append_empty_string(void) {
    ccary *ca = ccary_create(0);
    TEST_ASSERT_EQUAL_INT(0, ccary_append(ca, ""));
    TEST_ASSERT_EQUAL_size_t(1, ccary_get_size(ca));
    TEST_ASSERT_EQUAL_STRING("", ccary_get_at(ca, 0));
    ccary_destroy(ca);
    return 0;
}

static int test_ccary_clean(void) { /* NOLINT(readability-function-cognitive-complexity) */
    ccary *ca = ccary_create(0);
    TEST_ASSERT_EQUAL_INT(0, ccary_append(ca, "Hello"));
    TEST_ASSERT_EQUAL_INT(0, ccary_append(ca, "world"));
    TEST_ASSERT_EQUAL_size_t(2, ccary_get_size(ca));
    TEST_ASSERT_EQUAL_INT(0, ccary_clean(ca));
    TEST_ASSERT_EQUAL_size_t(0, ccary_get_size(ca));
    TEST_ASSERT_EQUAL_size_t(16, ccary_get_capacity(ca));
    TEST_ASSERT_EQUAL_INT(0, ccary_append(ca, "new"));
    TEST_ASSERT_EQUAL_size_t(1, ccary_get_size(ca));
    TEST_ASSERT_EQUAL_STRING("new", ccary_get_at(ca, 0));
    ccary_destroy(ca);
    return 0;
}

static int test_ccary_size(void) {
    ccary *ca = ccary_create(0);
    TEST_ASSERT_EQUAL_size_t(0, ccary_get_size(ca));
    TEST_ASSERT_EQUAL_INT(0, ccary_append(ca, "a"));
    TEST_ASSERT_EQUAL_size_t(1, ccary_get_size(ca));
    TEST_ASSERT_EQUAL_INT(0, ccary_append(ca, "b"));
    TEST_ASSERT_EQUAL_size_t(2, ccary_get_size(ca));
    ccary_destroy(ca);
    return 0;
}

static int g_foreach_count = 0;  /* NOLINT(cppcoreguidelines-avoid-non-const-global-variables) */
static void count_callback(const char *str) {
    (void)str;
    g_foreach_count++;
}

static int test_ccary_foreach(void) {
    ccary *ca = ccary_create(0);
    TEST_ASSERT_EQUAL_INT(0, ccary_append(ca, "a"));
    TEST_ASSERT_EQUAL_INT(0, ccary_append(ca, "b"));
    TEST_ASSERT_EQUAL_INT(0, ccary_append(ca, "c"));
    g_foreach_count = 0;
    TEST_ASSERT_EQUAL_INT(0, ccary_foreach(ca, count_callback));
    TEST_ASSERT_EQUAL(3, g_foreach_count);
    ccary_destroy(ca);
    return 0;
}

static int test_ccary_get_at_out_of_bounds(void) {
    ccary *ca = ccary_create(0);
    TEST_ASSERT_NULL(ccary_get_at(ca, 0));
    TEST_ASSERT_EQUAL_INT(0, ccary_append(ca, "a"));
    TEST_ASSERT_NULL(ccary_get_at(ca, 1));
    ccary_destroy(ca);
    return 0;
}

static int test_ccary_capacity_growth(void) {
    ccary *ca = ccary_create(2);
    TEST_ASSERT_EQUAL_size_t(2, ccary_get_capacity(ca));
    TEST_ASSERT_EQUAL_INT(0, ccary_append(ca, "a"));
    TEST_ASSERT_EQUAL_INT(0, ccary_append(ca, "b"));
    TEST_ASSERT_EQUAL_size_t(2, ccary_get_size(ca));
    TEST_ASSERT_EQUAL_size_t(2, ccary_get_capacity(ca));
    TEST_ASSERT_EQUAL_INT(0, ccary_append(ca, "c"));
    TEST_ASSERT_EQUAL_size_t(4, ccary_get_capacity(ca));
    ccary_destroy(ca);
    return 0;
}

static int test_ccary_null_params(void) { /* NOLINT(readability-function-cognitive-complexity) */
    ccary *ca = ccary_create(0);
    TEST_ASSERT_EQUAL_INT(-1, ccary_append(NULL, "str"));
    TEST_ASSERT_EQUAL_INT(-1, ccary_append(ca, NULL));
    TEST_ASSERT_EQUAL_INT(-1, ccary_foreach(NULL, count_callback));
    TEST_ASSERT_EQUAL_INT(-1, ccary_foreach(ca, NULL));
    TEST_ASSERT_EQUAL_INT(-1, ccary_clean(NULL));
    TEST_ASSERT_EQUAL_size_t(0, ccary_get_size(NULL));
    TEST_ASSERT_EQUAL_size_t(0, ccary_get_capacity(NULL));
    TEST_ASSERT_NULL(ccary_get_at(NULL, 0));
    TEST_ASSERT_NULL(ccary_iterator_create(NULL));
    TEST_ASSERT_EQUAL(-1, ccary_iterator_has_next(NULL));
    TEST_ASSERT_NULL(ccary_iterator_next(NULL));
    ccary_destroy(ca);
    return 0;
}

static int test_ccary_iterator_empty(void) {
    ccary *ca = ccary_create(0);
    ccary_iterator *it = ccary_iterator_create(ca);
    TEST_ASSERT_NOT_NULL(it);
    TEST_ASSERT_EQUAL(0, ccary_iterator_has_next(it));
    TEST_ASSERT_NULL(ccary_iterator_next(it));
    ccary_iterator_destroy(it);
    ccary_destroy(ca);
    return 0;
}

static int test_ccary_destroy_null(void) {
    ccary_destroy(NULL);  /* Should not crash */
    ccary_iterator_destroy(NULL);  /* Should not crash */
    return 0;
}

static int test_ccary_foreach_empty(void) {
    ccary *ca = ccary_create(0);
    g_foreach_count = 0;
    TEST_ASSERT_EQUAL_INT(0, ccary_foreach(ca, count_callback));
    TEST_ASSERT_EQUAL(0, g_foreach_count);
    ccary_destroy(ca);
    return 0;
}

static int test_ccary_iterator(void) { /* NOLINT(readability-function-cognitive-complexity) */
    ccary *ca = ccary_create(0);
    ccary_iterator *it = NULL;
    TEST_ASSERT_EQUAL_INT(0, ccary_append(ca, "a"));
    TEST_ASSERT_EQUAL_INT(0, ccary_append(ca, "b"));
    TEST_ASSERT_EQUAL_INT(0, ccary_append(ca, "c"));

    it = ccary_iterator_create(ca);
    TEST_ASSERT_NOT_NULL(it);
    TEST_ASSERT_EQUAL(1, ccary_iterator_has_next(it));
    TEST_ASSERT_EQUAL_STRING("a", ccary_iterator_next(it));
    TEST_ASSERT_EQUAL(1, ccary_iterator_has_next(it));
    TEST_ASSERT_EQUAL_STRING("b", ccary_iterator_next(it));
    TEST_ASSERT_EQUAL(1, ccary_iterator_has_next(it));
    TEST_ASSERT_EQUAL_STRING("c", ccary_iterator_next(it));
    TEST_ASSERT_EQUAL(0, ccary_iterator_has_next(it));
    TEST_ASSERT_NULL(ccary_iterator_next(it));

    ccary_iterator_destroy(it);
    ccary_destroy(ca);
    return 0;
}

typedef int (*test_func_t)(void);

int main(void) {
    static const test_func_t test_funcs[] = {
        test_ccary_create_destroy,
        test_ccary_create_with_capacity,
        test_ccary_append,
        test_ccary_append_empty_string,
        test_ccary_clean,
        test_ccary_size,
        test_ccary_foreach,
        test_ccary_get_at_out_of_bounds,
        test_ccary_capacity_growth,
        test_ccary_null_params,
        test_ccary_iterator_empty,
        test_ccary_destroy_null,
        test_ccary_foreach_empty,
        test_ccary_iterator,
    };

    static const char * const test_names[] = {
        "test_ccary_create_destroy",
        "test_ccary_create_with_capacity",
        "test_ccary_append",
        "test_ccary_append_empty_string",
        "test_ccary_clean",
        "test_ccary_size",
        "test_ccary_foreach",
        "test_ccary_get_at_out_of_bounds",
        "test_ccary_capacity_growth",
        "test_ccary_null_params",
        "test_ccary_iterator_empty",
        "test_ccary_destroy_null",
        "test_ccary_foreach_empty",
        "test_ccary_iterator",
    };

    TEST_START();
    for (size_t idx = 0; idx < sizeof(test_funcs) / sizeof(test_funcs[0]); idx++) {
        RUN_TEST_NAMED(test_funcs[idx], test_names[idx]);
    }
    TEST_END();
}
