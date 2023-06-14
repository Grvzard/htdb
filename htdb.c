
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "siphash.h"
#include "htdb.h"


static hash_t _xobjGenHash(void *obj) {
    return siphash(((xobj *)obj)->data, ((xobj *)obj)->len, "0123456789ABCDEF");
}

int xobjCmp(void *obj1_, void *obj2_) {
    xobj *obj1 = (xobj *)obj1_;
    xobj *obj2 = (xobj *)obj2_;
    if (obj1->type != obj2->type) {
        return 0;
    }
    if (obj1->len != obj2->len) {
        return 0;
    }
    if (memcmp(obj1->data, obj2->data, obj1->len) != 0) {
        return 0;
    }

    return 1;
}

xdb *xdbNew(char key_type, char value_type) {
    xdb *db = malloc(sizeof(xdb));
    db->table = dictNewCustom(xobjCmp, _xobjGenHash);

    return db;
}

void xdbFree(xdb *db) {
    dictFree(db->table);
    free(db);
}

xobj *xobjNew(uint8_t type, uint8_t *data, uint8_t len) {
    xobj *obj = malloc(sizeof(xobj) + len);
    obj->type = type;
    obj->len = len;
    memcpy(obj->data, data, len);

    return obj;
}

// inner dict will call free(obj) directly, so the impl here should not do anything more
void xobjFree(xobj *obj) {
    free(obj);
}

int _xdbSetIntBytes(xdb *db, uint64_t key_, char *value_, uint8_t value_len) {
    uint8_t key_len = sizeof(key_);

    xobj *keyobj = xobjNew(XOBJ_TYPE_INT, (uint8_t *)&key_, key_len);
    xobj *valobj = xobjNew(XOBJ_TYPE_BYTES, (uint8_t *)value_, value_len);

    dictSet(db->table, (void *)keyobj, (void *)valobj);

    return 1;
}

int _xdbSetIntInt(xdb *db, uint64_t key_, uint64_t value_) {
    uint8_t key_len = sizeof(key_);
    uint8_t value_len = sizeof(value_);

    xobj *keyobj = xobjNew(XOBJ_TYPE_INT, (uint8_t *)&key_, key_len);
    xobj *valobj = xobjNew(XOBJ_TYPE_INT, (uint8_t *)&value_, value_len);

    dictSet(db->table, (void *)keyobj, (void *)valobj);

    return 1;
}

int _xdbSetBytesBytes(xdb *db, char *key_, uint8_t key_len, char *value_, uint8_t value_len) {
    xobj *keyobj = xobjNew(XOBJ_TYPE_BYTES, (uint8_t *)key_, key_len);
    xobj *valobj = xobjNew(XOBJ_TYPE_BYTES, (uint8_t *)value_, value_len);

    dictSet(db->table, (void *)keyobj, (void *)valobj);

    return 1;
}

int _xdbGetIntBytes(xdb *db, uint64_t key_, char **value_, uint8_t *value_len) {
    uint8_t key_len = sizeof(key_);

    xobj *keyobj = xobjNew(XOBJ_TYPE_INT, (uint8_t *)&key_, key_len);

    if (!dictHas(db->table, keyobj)) {
        return 0;
    }
    xobj *valobj = dictGet(db->table, keyobj);

    *value_ = (char *)valobj->data;

    xobjFree(keyobj);
    return 1;
}

int _xdbGetBytesBytes(xdb *db, char *key_, uint8_t key_len, char **value_, uint8_t *value_len) {
    xobj *keyobj = xobjNew(XOBJ_TYPE_BYTES, (uint8_t *)key_, key_len);

    if (!dictHas(db->table, keyobj)) {
        return 0;
    }
    xobj *valobj = dictGet(db->table, keyobj);

    *value_ = (char *)valobj->data;

    free(keyobj);
    return 1;
}

int _xdbGetIntInt(xdb *db, uint64_t key_, uint64_t *value) {
    uint8_t key_len = sizeof(key_);

    xobj *keyobj = malloc(sizeof(xobj) + key_len);
    keyobj->type = XOBJ_TYPE_INT;
    keyobj->len = key_len;
    memcpy(keyobj->data, &key_, key_len);

    if (!dictHas(db->table, keyobj)) {
        return 0;
    }
    xobj *valobj = dictGet(db->table, keyobj);

    if (valobj->len == 1) {
        *value = *(uint8_t *)valobj->data;
    } else if (valobj->len == 2) {
        *value = *(uint16_t *)valobj->data;
    } else if (valobj->len == 4) {
        *value = *(uint32_t *)valobj->data;
    } else if (valobj->len == 8) {
        *value = *(uint64_t *)valobj->data;
    } else {
        assert(0);
    }

    free(keyobj);
    return 1;
}

#ifdef HTDB_TEST

#include <stdio.h>
void htdbTest() {
    xdb *db = xdbNew('i', 'b');

    char *value;
    uint8_t value_len;

    _xdbSetBytesBytes(db, "wb", 2, "hello2", 7);
    if (_xdbGetBytesBytes(db, "wb", 2, &value, &value_len) == 1) {
        printf("value: %s\n", value);
    }

    for (uint64_t i = 0; i < 100; i++) {
        if (i == 32) {
            _xdbSetIntBytes(db, i, "hello32", 8);
        } else {
            _xdbSetIntBytes(db, i, "hello", 6);
        }
    }
    if (_xdbGetIntBytes(db, 0, &value, &value_len) == 1) {
        printf("value: %s\n", value);
    }
    if (_xdbGetIntBytes(db, 32, &value, &value_len) == 1) {
        printf("value: %s\n", value);
    }

    if (_xdbGetBytesBytes(db, "wb", 2, &value, &value_len) == 1) {
        printf("value: %s\n", value);
    }

    xdbFree(db);
}
#endif
