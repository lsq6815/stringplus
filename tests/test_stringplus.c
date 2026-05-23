#include "unity.h"
#include "stringplus.h"
#include "ccary.h"
#include <stdlib.h>

static void test_indexof_found(void) {
    TEST_ASSERT_EQUAL(0, indexof("Hello", 'H'));
    TEST_ASSERT_EQUAL(4, indexof("Hello", 'o'));
    TEST_ASSERT_EQUAL(2, indexof("Hello", 'l'));
}

static void test_indexof_not_found(void) {
    TEST_ASSERT_EQUAL(NOP, indexof("Hello", 'x'));
    TEST_ASSERT_EQUAL(NOP, indexof("", 'a'));
}

static void test_indexamong_found(void) {
    TEST_ASSERT_EQUAL(5, indexamong("Hello, world", ","));
    TEST_ASSERT_EQUAL(5, indexamong("Hello; world", ";:"));
    TEST_ASSERT_EQUAL(0, indexamong("Hello", "H"));
}

static void test_indexamong_not_found(void) {
    TEST_ASSERT_EQUAL(NOP, indexamong("Hello", "xyz"));
    TEST_ASSERT_EQUAL(NOP, indexamong("", "abc"));
}

static void test_split_normal(void) {
    ccary *result = split("a-b-c", "-");
    TEST_ASSERT_NOT_NULL(result);
    TEST_ASSERT_EQUAL(3, ccary_get_size(result));
    TEST_ASSERT_EQUAL_STRING("a", ccary_get_at(result, 0));
    TEST_ASSERT_EQUAL_STRING("b", ccary_get_at(result, 1));
    TEST_ASSERT_EQUAL_STRING("c", ccary_get_at(result, 2));
    ccary_destroy(result);
}

static void test_split_multiple_delimiters(void) {
    ccary *result = split("8208180119-lsq_infosec", "-_");
    TEST_ASSERT_NOT_NULL(result);
    TEST_ASSERT_EQUAL(3, ccary_get_size(result));
    TEST_ASSERT_EQUAL_STRING("8208180119", ccary_get_at(result, 0));
    TEST_ASSERT_EQUAL_STRING("lsq", ccary_get_at(result, 1));
    TEST_ASSERT_EQUAL_STRING("infosec", ccary_get_at(result, 2));
    ccary_destroy(result);
}

static void test_split_consecutive_delimiters(void) {
    ccary *result = split("a--b", "-");
    TEST_ASSERT_NOT_NULL(result);
    TEST_ASSERT_EQUAL(2, ccary_get_size(result));
    TEST_ASSERT_EQUAL_STRING("a", ccary_get_at(result, 0));
    TEST_ASSERT_EQUAL_STRING("b", ccary_get_at(result, 1));
    ccary_destroy(result);
}

static void test_split_empty_string(void) {
    ccary *result = split("", "-");
    TEST_ASSERT_NOT_NULL(result);
    TEST_ASSERT_EQUAL(0, ccary_get_size(result));
    ccary_destroy(result);
}

static void test_join_normal(void) {
    ccary *ca = ccary_create(0);
    ccary_append(ca, "Hello");
    ccary_append(ca, "world");
    char *result = join(ca, ' ');
    TEST_ASSERT_EQUAL_STRING("Hello world", result);
    free(result);
    ccary_destroy(ca);
}

static void test_join_single_element(void) {
    ccary *ca = ccary_create(0);
    ccary_append(ca, "Hello");
    char *result = join(ca, ' ');
    TEST_ASSERT_EQUAL_STRING("Hello", result);
    free(result);
    ccary_destroy(ca);
}

void setUp(void) {}
void tearDown(void) {}

int main(void) {
    UNITY_BEGIN();

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

    return UNITY_END();
}
