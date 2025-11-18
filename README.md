xattrkv
=======

A key value store backed by Mac OS's HFS+ extended attributes, which are stored in a B-tree.

Example usage can be seen in test.c

```
  db = xattrkv_new("name");
```
Creates a new empty file.
```
  xattrkv_set(db, "key", "some long string");
```
Sets the key "key" to "some long string"
```
  value = xattrkv_get(db, "keyname");
```
Stores the value (if it exists) of key "keyname" in a string. Caller is responsible for freeing.
```
  xattrkv_keys(db, &keys, &nkeys);
```
Returns an array of strings of key names in (char **keys).

limitations
-----------
There are probably a lot of limitations. I am afraid to benchmark this on my computer because I don't know what effect storing lots of data in attributes will have on the over all filesystem :) I suggest making a separate disk image or sparse image to play with it on.

Individual values can be up to almost 2 gigabytes, but I haven't tried that, nor have I set any limitations on the size you can attempt to write. It's unclear if there is an OS imposed limit on the number of keys. I've been able to set 10 million.

If you create a store, then delete it, Mac OS seems to remember key names for that file (maybe not forever?) but doesn't keep any values.

If you do a directory listing in a directory where you stored lots of keys, it will take a long time.

Copying the file off the system won't also copy the extended attribute data.

Keys are limited by macOS to 127 characters (255 on most Linux filesystems). Exceeding this limit will fail with ENAMETOOLONG.

xattrkvtool
===========
compile with:
```
gcc xattrkvtool.c xattrkv.c -o xattrkvtool
```

create a datastore by touching a file
```
# touch dbstore 
```

set keys
```
# xattrkvtool dbstore set awesome "the file is zero size, so you can store an infinite amount of data probably"
```

list keys
```
# xattrkvtool dbstore keys
awesome
#
```

get values
```
# xattrkvtool dbstore get awesome
the file is zero size, so you can store an infinite amount of data probably
```

delete keys
```
# xattrkvtool dbstore del awesome
```
