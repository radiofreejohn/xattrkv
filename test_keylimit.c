#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "xattrkv.h"

int main(int argc, char* argv[]) {
    (void)argc;
    (void)argv;

    // Create test database
    int db = xattrkv_new("testdb_keylimit");
    if (db == -1) {
        db = xattrkv_open("testdb_keylimit");
        if (db == -1) {
            printf("Failed to create/open test database\n");
            return 1;
        }
    }

    // Test key lengths from 1 to 1024
    int max_working = 0;
    int first_fail = 0;

    for (int len = 1; len <= 1024; len++) {
        // Create key of length 'len'
        char *key = malloc(len + 1);
        memset(key, 'k', len);
        key[len] = '\0';

        int err = xattrkv_set(db, key, "test");

        if (err == 0) {
            max_working = len;
            // Clean up - delete the key
            xattrkv_del(db, key);
        } else {
            if (first_fail == 0) {
                first_fail = len;
                printf("First failure at key length %d: %s (errno %d)\n",
                       len, strerror(errno), errno);
            }
        }

        free(key);

        // Stop after finding the limit
        if (first_fail != 0 && len > first_fail + 10) {
            break;
        }
    }

    printf("\nMax working key length: %d characters\n", max_working);

    xattrkv_close(db);

    // Clean up test file
    remove("testdb_keylimit");

    return 0;
}
