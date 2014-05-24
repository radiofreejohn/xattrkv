#include "xattrkv.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char* argv[]) {
    int err;
    int db = xattrkv_open(argv[1]);

    if (db == -1) {
        printf("can't open store. create a file with touch\n");
    }
    if (argc > 2) {
        if (strcmp(argv[2], "set") == 0) {
            if (argc != 5) {
                printf("xattrkvtool file set key value\n");
            } else {
                err = xattrkv_set(db, argv[3], argv[4]);
                if (err == -1) {
                    printf("Error: %s\n", strerror(errno));
                }
            }
        }
        if (strcmp(argv[2], "get") == 0) {
            if (argc != 4) {
                printf("xattrkvtool file get key\n");
            } else {
                char *value = xattrkv_get(db, argv[3]);
                if (value == NULL) {
                   printf("Error: %s\n", strerror(errno));
                } else {
                    printf("%s\n", value);
                }
                free(value);
            }
        }
        if (strcmp(argv[2], "del") == 0) {
            if (argc != 4) {
                printf("xattrkvtool file del key\n");
            } else {
                err = xattrkv_del(db, argv[3]);
                if (err == -1) {
                    printf("Error: %s\n", strerror(errno));
                }
            }
        }
        if (strcmp(argv[2], "keys") == 0) {
            char **keys;
            size_t nkeys;
            xattrkv_keys(db, &keys, &nkeys);
            for (int i = 0; i < nkeys; i++) {
                printf("%s\n", keys[i]);
                free(keys[i]);
            }
            free(keys);
        }
    }
    return 0; 
}
