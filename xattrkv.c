#include <sys/xattr.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
// for O_RDWR
#include <fcntl.h>
// for fpathconf
#include <unistd.h>
// for pow
#include <math.h>
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
// value must be less than about 2 gigabytes
int xattrkv_set(int db, char *key, char *value) {
    int retval;
    retval = fsetxattr(db, key, (char *)value, strlen(value), 0, 0);
    return retval;
}

char *xattrkv_get(int db, char *key) {
    size_t size;
    size = (size_t) fgetxattr(db, key, NULL, 0, 0, 0);
    char *value = malloc(size + 1);
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
void xattrkv_keys(int db, char ***keys, size_t *nkeys) {
    // flistxattr gives a buffer of null terminated strings

    // find out how much space to allocate
    size_t size = (size_t) flistxattr(db, NULL, 0, 0);
    char *buffer = malloc(size);
    flistxattr(db, buffer, size, 0);

    size_t keycount = 0;
    
    for (int i = 0; i < size; i++) {
        if (buffer[i] == '\0') {
            keycount++;
        }
    }
    *nkeys = keycount;
    *keys = malloc(sizeof(char*) * keycount);

    size_t index = 0;
    size_t bufpos = 0;
    // this is probably insane
    while (index != keycount) {
        *(*keys + index) = strdup(buffer+bufpos);
        bufpos = bufpos + strlen(*(*keys+index)) + 1;
        index = index + 1;
    }
}
