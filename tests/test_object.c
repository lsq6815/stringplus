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

typedef int (*test_func_t)(void);

int main(void) {
    static const test_func_t test_funcs[] = {
        test_object_create,
        test_object_retain_release,
        test_object_release_null,
    };

    static const char * const test_names[] = {
        "test_object_create",
        "test_object_retain_release",
        "test_object_release_null",
    };

    TEST_START();
    for (size_t idx = 0; idx < sizeof(test_funcs) / sizeof(test_funcs[0]); idx++) {
        RUN_TEST_NAMED(test_funcs[idx], test_names[idx]);
    }
    TEST_END();
}
