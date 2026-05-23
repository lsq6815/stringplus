#include "test_utils.h"
#include "object.h"
#include "obj_class.h"

static int test_object_create(void) {
    object *obj = object_create();
    TEST_ASSERT_NOT_NULL(obj);
    TEST_ASSERT_EQUAL_STRING("Object", obj_class_name(obj));
    obj_release(obj);
    return 0;
}

static int test_object_retain_release(void) {
    object *obj = object_create();
    TEST_ASSERT_NOT_NULL(obj);
    
    obj_retain(obj);
    obj_retain(obj);
    obj_release(obj);
    obj_release(obj);
    /* 还有一次引用，释放不应崩溃 */
    obj_release(obj);
    return 0;
}

static int test_object_release_null(void) {
    obj_release(NULL);  /* Should not crash */
    obj_retain(NULL);   /* Should not crash */
    return 0;
}

static int test_object_describe(void) {
    object *obj = object_create();
    TEST_ASSERT_NOT_NULL(obj);
    TEST_ASSERT_EQUAL_STRING("Object", obj_describe(obj));
    obj_release(obj);
    return 0;
}

static int test_object_describe_null(void) {
    TEST_ASSERT_NULL(obj_describe(NULL));
    return 0;
}

static int test_object_equals(void) {
    object *obj1 = object_create();
    object *obj2 = object_create();
    TEST_ASSERT_NOT_NULL(obj1);
    TEST_ASSERT_NOT_NULL(obj2);
    TEST_ASSERT_EQUAL_INT(1, obj_equals(obj1, obj1));
    TEST_ASSERT_EQUAL_INT(0, obj_equals(obj1, obj2));
    obj_release(obj1);
    obj_release(obj2);
    return 0;
}

static int test_object_equals_null(void) {
    object *obj = object_create();
    TEST_ASSERT_EQUAL_INT(-1, obj_equals(NULL, obj));
    TEST_ASSERT_EQUAL_INT(-1, obj_equals(obj, NULL));
    obj_release(obj);
    return 0;
}

static int test_object_compare(void) {
    object *obj1 = object_create();
    object *obj2 = object_create();
    TEST_ASSERT_NOT_NULL(obj1);
    TEST_ASSERT_NOT_NULL(obj2);
    TEST_ASSERT_EQUAL_INT(0, obj_compare(obj1, obj1));
    TEST_ASSERT(obj_compare(obj1, obj2) != 0);
    TEST_ASSERT_EQUAL_INT(obj_compare(obj1, obj2), -obj_compare(obj2, obj1));
    obj_release(obj1);
    obj_release(obj2);
    return 0;
}

static int test_object_hash(void) {
    object *obj = object_create();
    TEST_ASSERT_NOT_NULL(obj);
    TEST_ASSERT_EQUAL((size_t)obj, obj_hash(obj));
    TEST_ASSERT_EQUAL((size_t)0, obj_hash(NULL));
    obj_release(obj);
    return 0;
}

typedef int (*test_func_t)(void);

int main(void) {
    static const test_func_t test_funcs[] = {
        test_object_create,
        test_object_retain_release,
        test_object_release_null,
        test_object_describe,
        test_object_describe_null,
        test_object_equals,
        test_object_equals_null,
        test_object_compare,
        test_object_hash,
    };

    static const char * const test_names[] = {
        "test_object_create",
        "test_object_retain_release",
        "test_object_release_null",
        "test_object_describe",
        "test_object_describe_null",
        "test_object_equals",
        "test_object_equals_null",
        "test_object_compare",
        "test_object_hash",
    };

    TEST_START();
    for (size_t idx = 0; idx < sizeof(test_funcs) / sizeof(test_funcs[0]); idx++) {
        RUN_TEST_NAMED(test_funcs[idx], test_names[idx]);
    }
    TEST_END();
}
