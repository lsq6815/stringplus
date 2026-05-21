#include "test_utils.h"
#include "stringplus.h"
#include "ccary.h"

static int test_indexof_found(void) {
    TEST_ASSERT_EQUAL_size_t(0, indexof("Hello", 'H'));
    TEST_ASSERT_EQUAL_size_t(4, indexof("Hello", 'o'));
    TEST_ASSERT_EQUAL_size_t(2, indexof("Hello", 'l'));
    return 0;
}

static int test_indexof_not_found(void) {
    TEST_ASSERT_EQUAL_size_t(NOP, indexof("Hello", 'x'));
    TEST_ASSERT_EQUAL_size_t(NOP, indexof("", 'a'));
    return 0;
}

static int test_indexamong_found(void) {
    TEST_ASSERT_EQUAL_size_t(5, indexamong("Hello, world", ","));
    TEST_ASSERT_EQUAL_size_t(5, indexamong("Hello; world", ";:"));
    TEST_ASSERT_EQUAL_size_t(0, indexamong("Hello", "H"));
    return 0;
}

static int test_indexamong_not_found(void) {
    TEST_ASSERT_EQUAL_size_t(NOP, indexamong("Hello", "xyz"));
    TEST_ASSERT_EQUAL_size_t(NOP, indexamong("", "abc"));
    return 0;
}

static int test_split_normal(void) {
    pccary result = split("a-b-c", "-");
    TEST_ASSERT_NOT_NULL(result);
    TEST_ASSERT_EQUAL_size_t(3, ccary_size(result));
    TEST_ASSERT_EQUAL_STRING("a", result->data[0]);
    TEST_ASSERT_EQUAL_STRING("b", result->data[1]);
    TEST_ASSERT_EQUAL_STRING("c", result->data[2]);
    ccary_destroy(result);
    return 0;
}

static int test_split_multiple_delimiters(void) {
    pccary result = split("8208180119-lsq_infosec", "-_");
    TEST_ASSERT_NOT_NULL(result);
    TEST_ASSERT_EQUAL_size_t(3, ccary_size(result));
    TEST_ASSERT_EQUAL_STRING("8208180119", result->data[0]);
    TEST_ASSERT_EQUAL_STRING("lsq", result->data[1]);
    TEST_ASSERT_EQUAL_STRING("infosec", result->data[2]);
    ccary_destroy(result);
    return 0;
}

static int test_split_consecutive_delimiters(void) {
    pccary result = split("a--b", "-");
    TEST_ASSERT_NOT_NULL(result);
    TEST_ASSERT_EQUAL_size_t(2, ccary_size(result));
    TEST_ASSERT_EQUAL_STRING("a", result->data[0]);
    TEST_ASSERT_EQUAL_STRING("b", result->data[1]);
    ccary_destroy(result);
    return 0;
}

static int test_split_empty_string(void) {
    pccary result = split("", "-");
    TEST_ASSERT_NOT_NULL(result);
    TEST_ASSERT_EQUAL_size_t(0, ccary_size(result));
    ccary_destroy(result);
    return 0;
}

static int test_join_normal(void) {
    pccary pcca = ccary_init();
    ccary_append(pcca, "Hello");
    ccary_append(pcca, "world");
    char *result = join(pcca, ' ');
    TEST_ASSERT_EQUAL_STRING("Hello world", result);
    free(result);
    ccary_destroy(pcca);
    return 0;
}

static int test_join_single_element(void) {
    pccary pcca = ccary_init();
    ccary_append(pcca, "Hello");
    char *result = join(pcca, ' ');
    TEST_ASSERT_EQUAL_STRING("Hello", result);
    free(result);
    ccary_destroy(pcca);
    return 0;
}

int main(void) {
    TEST_START();
    RUN_TEST(test_indexof_found);
    RUN_TEST(test_indexof_not_found);
    RUN_TEST(test_indexamong_found);
    RUN_TEST(test_indexamong_not_found);
    RUN_TEST(test_split_normal);
    RUN_TEST(test_split_multiple_delimiters);
    RUN_TEST(test_split_consecutive_delimiters);
    RUN_TEST(test_split_empty_string);
    RUN_TEST(test_join_normal);
    RUN_TEST(test_join_single_element);
    TEST_END();
}
