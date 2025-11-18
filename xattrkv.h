#ifndef _XATTRKV
#define _XATTRKV
#include <stddef.h>

int   xattrkv_new(char *dbname);
int   xattrkv_open(char *dbname);
int   xattrkv_set(int db, char *key, char *value);
char *xattrkv_get(int db, char *key);
int   xattrkv_del(int db, char *key);
int   xattrkv_close(int db);
int   xattrkv_keys(int db, char ***keys, size_t *nkeys);

#endif
