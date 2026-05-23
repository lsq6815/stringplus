#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/object.h"
#include "../include/obj_class.h"

/* object.c 内部可访问完整定义 */
struct object {
    const obj_class *isa;
    size_t ref_count;
};

/* 基类默认实现 */
static void object_dealloc(object *obj) {
    if (obj == NULL) {
        return;
    }
    free(obj);
}

static const char *object_describe(const object *obj) {
    if (obj == NULL) {
        return NULL;
    }
    return obj->isa->name;
}

static int object_equals(const object *a, const object *b) {
    if (a == NULL || b == NULL) {
        return -1;
    }
    return a == b ? 1 : 0;
}

static int object_compare(const object *a, const object *b) {
    if (a == NULL || b == NULL) {
        return -1;
    }
    if (a < b) {
        return -1;
    }
    if (a > b) {
        return 1;
    }
    return 0;
}

static size_t object_hash(const object *obj) {
    if (obj == NULL) {
        return 0;
    }
    return (size_t)obj;
}

static const obj_vtable object_vtable = {
    .dealloc = object_dealloc,
    .describe = object_describe,
    .equals = object_equals,
    .compare = object_compare,
    .hash = object_hash,
};

const obj_class ObjectClass = {
    .name = "Object",
    .parent = NULL,
    .vtable = object_vtable,
};

object *object_create(void) {
    object *obj = malloc(sizeof(object));
    if (obj == NULL) {
        return NULL;
    }
    obj->isa = &ObjectClass;
    obj->ref_count = 1;
    return obj;
}

void obj_retain(object *obj) {
    if (obj == NULL) {
        return;
    }
    obj->ref_count++;
}

void obj_release(object *obj) {
    if (obj == NULL) {
        return;
    }
    obj->ref_count--;
    if (obj->ref_count == 0) {
        obj->isa->vtable.dealloc(obj);
    }
}

const char *obj_describe(object *obj) {
    if (obj == NULL || obj->isa == NULL) {
        return NULL;
    }
    return obj->isa->vtable.describe(obj);
}

int obj_equals(object *a, object *b) {
    if (a == NULL || b == NULL || a->isa == NULL) {
        return -1;
    }
    return a->isa->vtable.equals(a, b);
}

int obj_compare(object *a, object *b) {
    if (a == NULL || b == NULL || a->isa == NULL) {
        return -1;
    }
    return a->isa->vtable.compare(a, b);
}

size_t obj_hash(object *obj) {
    if (obj == NULL || obj->isa == NULL) {
        return 0;
    }
    return obj->isa->vtable.hash(obj);
}

int obj_is_kind_of_class(const object *obj, const obj_class *cls) {
    const obj_class *current = NULL;
    if (obj == NULL || cls == NULL || obj->isa == NULL) {
        return 0;
    }
    current = obj->isa;
    while (current != NULL) {
        if (current == cls) {
            return 1;
        }
        current = current->parent;
    }
    return 0;
}

int obj_is_member_of_class(const object *obj, const obj_class *cls) {
    if (obj == NULL || cls == NULL || obj->isa == NULL) {
        return 0;
    }
    return obj->isa == cls ? 1 : 0;
}

const char *obj_class_name(const object *obj) {
    if (obj == NULL || obj->isa == NULL) {
        return NULL;
    }
    return obj->isa->name;
}

const obj_class *obj_get_class(const object *obj) {
    if (obj == NULL) {
        return NULL;
    }
    return obj->isa;
}
