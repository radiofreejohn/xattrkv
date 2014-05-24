#include <stdlib.h>
#include <stdio.h>
#include "xattrkv.h"

int main(int argc, char* argv[]) {
    int db;
    db = xattrkv_new("testdb");
    if (db != -1) {
        xattrkv_set(db, "test", "this is a test");
        xattrkv_set(db, "testing", "this is a test");
        xattrkv_set(db, "testone", "this is a test");
        xattrkv_set(db, "testtwo", "this is a test");
    } else {
        printf("file exists, opening it instead!\n");
        db = xattrkv_open("testdb");
        char *val = xattrkv_get(db, "test");
        printf("got value from \"test\": %s\n", val);
        free(val);
    }
    char **keys;
    size_t nkeys;
    xattrkv_keys(db, &keys, &nkeys);
    printf("nkeys: %ld\n", nkeys);
    for (int i = 0; i < nkeys; i++) {
        printf("key: %s\n", keys[i]);
    }
    close(db);
    return 0;
}
