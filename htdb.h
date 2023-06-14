
#ifndef _HTDB_H_
#define _HTDB_H_

#include <stdint.h>
#include "dict.h"

typedef uint64_t xhash_t;

// >> settings
#define HTDB_TEST
// << settings

typedef enum {
    XOBJ_TYPE_INT = 0,
    XOBJ_TYPE_BYTES = 1,
    XOBJ_TYPE_FLOAT = 2,
    XOBJ_TYPE_VARINT = 3,
    // XOBJ_TYPE_BOOL,
    // XOBJ_TYPE_NULL,
    // XOBJ_TYPE_ARRAY,
    // XOBJ_TYPE_OBJECT,
} xobj_type;

typedef struct {
    uint8_t type;  // xobj_type
    uint8_t len;
    xhash_t hash;
    uint8_t data[];
} xobj;

typedef struct {
    Dict *table;

} xdb;

xdb *xdbNew(char key_type, char val_type);
void xdbFree(xdb *db);
// void xdbDump(xdb *db, char *fpath);


#ifdef HTDB_TEST
void htdbTest(void);
#endif  // HTDB_TEST

#endif  // _HTDB_H_
