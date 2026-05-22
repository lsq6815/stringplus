#include "test_utils.h"
#include "ccary.h"

static int test_ccary_init_destroy(void) {
    pccary pcca = ccary_init();
    TEST_ASSERT_NOT_NULL(pcca);
    TEST_ASSERT_EQUAL_size_t(0, ccary_size(pcca));
    TEST_ASSERT(pcca->data == NULL);
    ccary_destroy(pcca);
    return 0;
}

static int test_ccary_append(void) {
    pccary pcca = ccary_init();
    ccary_append(pcca, "Hello");
    TEST_ASSERT_EQUAL_size_t(1, ccary_size(pcca));
    TEST_ASSERT_EQUAL_STRING("Hello", pcca->data[0]);
    ccary_append(pcca, "world");
    TEST_ASSERT_EQUAL_size_t(2, ccary_size(pcca));
    TEST_ASSERT_EQUAL_STRING("world", pcca->data[1]);
    ccary_destroy(pcca);
    return 0;
}

static int test_ccary_append_empty_string(void) {
    pccary pcca = ccary_init();
    ccary_append(pcca, "");
    TEST_ASSERT_EQUAL_size_t(1, ccary_size(pcca));
    TEST_ASSERT_EQUAL_STRING("", pcca->data[0]);
    ccary_destroy(pcca);
    return 0;
}

static int test_ccary_clean(void) {
    pccary pcca = ccary_init();
    ccary_append(pcca, "Hello");
    ccary_append(pcca, "world");
    TEST_ASSERT_EQUAL_size_t(2, ccary_size(pcca));
    ccary_clean(pcca);
    TEST_ASSERT_EQUAL_size_t(0, ccary_size(pcca));
    TEST_ASSERT(pcca->data == NULL);
    ccary_append(pcca, "new");
    TEST_ASSERT_EQUAL_size_t(1, ccary_size(pcca));
    TEST_ASSERT_EQUAL_STRING("new", pcca->data[0]);
    ccary_destroy(pcca);
    return 0;
}

static int test_ccary_size(void) {
    pccary pcca = ccary_init();
    TEST_ASSERT_EQUAL_size_t(0, ccary_size(pcca));
    ccary_append(pcca, "a");
    TEST_ASSERT_EQUAL_size_t(1, ccary_size(pcca));
    ccary_append(pcca, "b");
    TEST_ASSERT_EQUAL_size_t(2, ccary_size(pcca));
    ccary_destroy(pcca);
    return 0;
}

static int g_foreach_count = 0;  /* NOLINT(cppcoreguidelines-avoid-non-const-global-variables) */
static void count_callback(void *str) {
    (void)str;
    g_foreach_count++;
}

static int test_ccary_foreach(void) {
    pccary pcca = ccary_init();
    ccary_append(pcca, "a");
    ccary_append(pcca, "b");
    ccary_append(pcca, "c");
    g_foreach_count = 0;
    ccary_foreach(pcca, count_callback);
    TEST_ASSERT_EQUAL(3, g_foreach_count);
    ccary_destroy(pcca);
    return 0;
}

typedef int (*test_func_t)(void);

int main(void) {
    static const test_func_t test_funcs[] = {
        test_ccary_init_destroy,
        test_ccary_append,
        test_ccary_append_empty_string,
        test_ccary_clean,
        test_ccary_size,
        test_ccary_foreach,
    };

    static const char * const test_names[] = {
        "test_ccary_init_destroy",
        "test_ccary_append",
        "test_ccary_append_empty_string",
        "test_ccary_clean",
        "test_ccary_size",
        "test_ccary_foreach",
    };

    TEST_START();
    for (size_t idx = 0; idx < sizeof(test_funcs) / sizeof(test_funcs[0]); idx++) {
        RUN_TEST_NAMED(test_funcs[idx], test_names[idx]);
    }
    TEST_END();
}
