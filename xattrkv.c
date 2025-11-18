#include <sys/xattr.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
// for O_RDWR
#include <fcntl.h>
// for fpathconf
#include <unistd.h>
// for mode on files
#include <sys/types.h>
#include <sys/stat.h>

int xattrkv_open(char *dbname) {
    struct stat dbs;
    int ok = stat(dbname, &dbs);
    if (ok == 0) {
        mode_t rw = (S_IWUSR|S_IRUSR);
        if ((dbs.st_mode & rw) == rw) {
            int db = open(dbname, O_RDWR, S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP);
            return db;
        }
    }
    // failed
    return -1;
}

int xattrkv_new(char *dbname) {
    // check if file exists
    struct stat dbs;
    int ok = stat(dbname, &dbs);
    if (ok != 0) {
        int db = open(dbname, O_CREAT|O_RDWR, S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP);
        return db;
    }
    return -1;
}

// 0 on success -1 on failure
// keys longer than 127 characters on macOS (255 on Linux) will fail with ENAMETOOLONG
// value must be less than about 2 gigabytes
int xattrkv_set(int db, char *key, char *value) {
    int retval;
    retval = fsetxattr(db, key, (char *)value, strlen(value), 0, 0);
    return retval;
}

char *xattrkv_get(int db, char *key) {
    ssize_t size = fgetxattr(db, key, NULL, 0, 0, 0);
    if (size == -1) {
        return NULL;
    }
    char *value = malloc(size + 1);
    if (value == NULL) {
        return NULL;
    }
    ssize_t ok = fgetxattr(db, key, value, size, 0, 0);
    if (ok == -1) {
        free(value);
        return NULL;
    }
    value[size] = '\0';
    return value;
}

// 0 is success -1 is failure
int xattrkv_del(int db, char *key) {
    return fremovexattr(db, key, 0);
}

// caller can call close directly
int xattrkv_close(int db) {
    return close(db);
}

// list all keys, caller responsible for freeing
// returns 0 on success, -1 on failure
int xattrkv_keys(int db, char ***keys, size_t *nkeys) {
    // flistxattr gives a buffer of null terminated strings

    // find out how much space to allocate
    ssize_t size = flistxattr(db, NULL, 0, 0);
    if (size == -1) {
        *nkeys = 0;
        *keys = NULL;
        return -1;
    }

    // handle empty case
    if (size == 0) {
        *nkeys = 0;
        *keys = NULL;
        return 0;
    }

    char *buffer = malloc(size);
    if (buffer == NULL) {
        *nkeys = 0;
        *keys = NULL;
        return -1;
    }

    if (flistxattr(db, buffer, size, 0) == -1) {
        free(buffer);
        *nkeys = 0;
        *keys = NULL;
        return -1;
    }

    size_t keycount = 0;

    for (ssize_t i = 0; i < size; i++) {
        if (buffer[i] == '\0') {
            keycount++;
        }
    }
    *nkeys = keycount;
    *keys = malloc(sizeof(char*) * keycount);
    if (*keys == NULL) {
        free(buffer);
        *nkeys = 0;
        return -1;
    }

    size_t index = 0;
    size_t bufpos = 0;
    while (index != keycount) {
        (*keys)[index] = strdup(buffer + bufpos);
        bufpos = bufpos + strlen((*keys)[index]) + 1;
        index = index + 1;
    }
    free(buffer);
    return 0;
}
