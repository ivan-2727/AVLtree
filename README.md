## AVL tree
This is an implementation of self-balancing AVL tree data structure in Go and C++ (similar things with Red-Black tree are `std::map` in C++ and `TreeMap` in Java), from scratch, for self-educational purposes 
- `insert(key)` adds a new vertex with given key, or increments counter if this key is already present
- `find(key)` returns pointer to the vertex with given key if it's present, nil otherwise
- `min()`, `max()` return pointer to the vertex with min/max key
- `next(vertex)` moves iterator from the given vertex to the next one, in increasing order (aka `++` in C++ or `next()` in Java). 

See also my project with a more advanced data structure skiplist https://github.com/ivan-2727/skiplist
