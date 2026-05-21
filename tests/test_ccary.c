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

static int foreach_count = 0;
static void count_callback(void *str) {
    (void)str;
    foreach_count++;
}

static int test_ccary_foreach(void) {
    pccary pcca = ccary_init();
    ccary_append(pcca, "a");
    ccary_append(pcca, "b");
    ccary_append(pcca, "c");
    foreach_count = 0;
    ccary_foreach(pcca, count_callback);
    TEST_ASSERT_EQUAL(3, foreach_count);
    ccary_destroy(pcca);
    return 0;
}

int main(void) {
    TEST_START();
    RUN_TEST(test_ccary_init_destroy);
    RUN_TEST(test_ccary_append);
    RUN_TEST(test_ccary_append_empty_string);
    RUN_TEST(test_ccary_clean);
    RUN_TEST(test_ccary_size);
    RUN_TEST(test_ccary_foreach);
    TEST_END();
}
