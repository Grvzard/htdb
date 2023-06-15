
#ifndef _HTDB_H_
#define _HTDB_H_
#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "dict.h"

typedef uint64_t xhash_t;

// >> settings
#define HTDB_TEST
// << settings

typedef enum {
    XOBJ_TYPE_INT = 0,
    XOBJ_TYPE_BYTES = 1,
    // XOBJ_TYPE_FLOAT = 2,
    // XOBJ_TYPE_VARINT = 3,
    // XOBJ_TYPE_BOOL,
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
int _xdbSetIntBytes(xdb *db, uint64_t key_, const char *value_, uint8_t value_len);
int _xdbSetIntInt(xdb *db, uint64_t key_, uint64_t value_);
int _xdbSetBytesBytes(xdb *db, const char *key_, uint8_t key_len, const char *value_, uint8_t value_len);
int _xdbGetIntBytes(xdb *db, uint64_t key_, char **value_, uint8_t *value_len);
int _xdbGetBytesBytes(xdb *db, const char *key_, uint8_t key_len, char **value_, uint8_t *value_len);
int _xdbGetIntInt(xdb *db, uint64_t key_, uint64_t *value);


#ifdef HTDB_TEST
void htdbTest(void);
#endif  // HTDB_TEST

#ifdef __cplusplus
}
#endif
#endif  // _HTDB_H_
