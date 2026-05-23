#include "unity.h"
#include "object.h"
#include "obj_class.h"

static void test_object_create(void) {
    object *obj = object_create();
    TEST_ASSERT_NOT_NULL(obj);
    TEST_ASSERT_EQUAL_STRING("Object", obj_class_name(obj));
    obj_release(obj);
}

static void test_object_retain_release(void) {
    object *obj = object_create();
    TEST_ASSERT_NOT_NULL(obj);
    
    obj_retain(obj);
    obj_retain(obj);
    obj_release(obj);
    obj_release(obj);
    obj_release(obj);
}

static void test_object_release_null(void) {
    obj_release(NULL);
    obj_retain(NULL);
}

static void test_object_describe(void) {
    object *obj = object_create();
    TEST_ASSERT_NOT_NULL(obj);
    TEST_ASSERT_EQUAL_STRING("Object", obj_describe(obj));
    obj_release(obj);
}

static void test_object_describe_null(void) {
    TEST_ASSERT_NULL(obj_describe(NULL));
}

static void test_object_equals(void) {
    object *obj1 = object_create();
    object *obj2 = object_create();
    TEST_ASSERT_NOT_NULL(obj1);
    TEST_ASSERT_NOT_NULL(obj2);
    TEST_ASSERT_EQUAL_INT(1, obj_equals(obj1, obj1));
    TEST_ASSERT_EQUAL_INT(0, obj_equals(obj1, obj2));
    obj_release(obj1);
    obj_release(obj2);
}

static void test_object_equals_null(void) {
    object *obj = object_create();
    TEST_ASSERT_EQUAL_INT(-1, obj_equals(NULL, obj));
    TEST_ASSERT_EQUAL_INT(-1, obj_equals(obj, NULL));
    obj_release(obj);
}

static void test_object_compare(void) {
    object *obj1 = object_create();
    object *obj2 = object_create();
    TEST_ASSERT_NOT_NULL(obj1);
    TEST_ASSERT_NOT_NULL(obj2);
    TEST_ASSERT_EQUAL_INT(0, obj_compare(obj1, obj1));
    TEST_ASSERT_TRUE(obj_compare(obj1, obj2) != 0);
    TEST_ASSERT_EQUAL_INT(obj_compare(obj1, obj2), -obj_compare(obj2, obj1));
    obj_release(obj1);
    obj_release(obj2);
}

static void test_object_hash(void) {
    object *obj = object_create();
    TEST_ASSERT_NOT_NULL(obj);
    TEST_ASSERT_EQUAL((size_t)obj, obj_hash(obj));
    TEST_ASSERT_EQUAL((size_t)0, obj_hash(NULL));
    obj_release(obj);
}

static void test_object_is_member_of_class(void) {
    object *obj = object_create();
    TEST_ASSERT_NOT_NULL(obj);
    TEST_ASSERT_EQUAL_INT(1, obj_is_member_of_class(obj, &ObjectClass));
    TEST_ASSERT_EQUAL_INT(0, obj_is_member_of_class(obj, NULL));
    TEST_ASSERT_EQUAL_INT(0, obj_is_member_of_class(NULL, &ObjectClass));
    obj_release(obj);
}

static void test_object_is_kind_of_class(void) {
    object *obj = object_create();
    TEST_ASSERT_NOT_NULL(obj);
    TEST_ASSERT_EQUAL_INT(1, obj_is_kind_of_class(obj, &ObjectClass));
    TEST_ASSERT_EQUAL_INT(0, obj_is_kind_of_class(obj, NULL));
    TEST_ASSERT_EQUAL_INT(0, obj_is_kind_of_class(NULL, &ObjectClass));
    obj_release(obj);
}

static void test_object_get_class(void) {
    object *obj = object_create();
    TEST_ASSERT_NOT_NULL(obj);
    TEST_ASSERT_TRUE(obj_get_class(obj) == &ObjectClass);
    TEST_ASSERT_NULL(obj_get_class(NULL));
    obj_release(obj);
}

static void test_object_class_name(void) {
    object *obj = object_create();
    TEST_ASSERT_NOT_NULL(obj);
    TEST_ASSERT_EQUAL_STRING("Object", obj_class_name(obj));
    TEST_ASSERT_NULL(obj_class_name(NULL));
    obj_release(obj);
}

void setUp(void) {}
void tearDown(void) {}

int main(void) {
    UNITY_BEGIN();

    RUN_TEST(test_object_create);
    RUN_TEST(test_object_retain_release);
    RUN_TEST(test_object_release_null);
    RUN_TEST(test_object_describe);
    RUN_TEST(test_object_describe_null);
    RUN_TEST(test_object_equals);
    RUN_TEST(test_object_equals_null);
    RUN_TEST(test_object_compare);
    RUN_TEST(test_object_hash);
    RUN_TEST(test_object_is_member_of_class);
    RUN_TEST(test_object_is_kind_of_class);
    RUN_TEST(test_object_get_class);
    RUN_TEST(test_object_class_name);

    return UNITY_END();
}
