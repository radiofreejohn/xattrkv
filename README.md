xattrkv
=======

A key value store backed by Mac OS's HFS+ extended attributes

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
There are probably a lot of limitations. I am afraid to benchmark this on my computer because I don't know what effect storing lots of data in attributes will have on the over all filesystem :)

Individual values can be up to almost 2 gigabytes, but I haven't tried that, nor have I set any limitations on the size you can attempt to write.
