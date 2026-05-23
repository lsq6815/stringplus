#ifndef OBJ_CLASS_H
#define OBJ_CLASS_H

#include <stddef.h>

typedef struct object object;
typedef struct obj_class obj_class;

typedef struct obj_vtable {
    void (*dealloc)(object *obj);
    const char *(*describe)(const object *obj);
    int (*equals)(const object *a, const object *b);
    int (*compare)(const object *a, const object *b);
    size_t (*hash)(const object *obj);
} obj_vtable;

struct obj_class {
    const char *name;
    const obj_class *parent;
    const obj_vtable vtable;
};

/* 全局基类定义，供子类引用 */
extern const obj_class ObjectClass;

#endif
