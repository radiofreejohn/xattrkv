xattrkv
=======

A key value store backed by Mac OS's HFS+ extended attributes

Example usage can be seen in test.c


  db = xattrkv_new("name");

Creates a new empty file.

  value = xattrkv_get(db, "keyname");

Stores the value (if it exists) of key "keyname" in a string. Caller is responsible for freeing.

  xattrkv_keys(db, &keys, &nkeys);
  
Returns an array of strings of key names in (char **keys).
