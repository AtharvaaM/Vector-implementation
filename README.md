# Generic Vector Library in C

A type-generic dynamic array for C using `void*` internally, with macro wrappers for clean usage.

---

## Files

| File | Purpose |
|------|---------|
| `vectors.h` | Struct definitions and function declarations |
| `vectors.c` | All function implementations |
| `vectors_macros.h` | Macro wrappers for ergonomic usage |

Include all three in your project. Always include `vectors_macros.h` in your source files.

---

## Creating a Vector

```c
// Default (uses malloc/memcpy internally)
vector* v = V_CREATE(int);

// Custom copy/destroy/allocator functions
vector* v = V_CREATE_CUSTOM(MyType, my_copy, my_destroy, my_alloc, my_realloc, my_free);
```

Pass `NULL` for any custom function you don't need — it falls back to the default.

---

## Push, Pop, Get

```c
V_PUSH(v, 42);              // push a value (handles literals safely)
V_POP(v);                   // remove and destroy last element

int* p = V_GET(v, 0, int);  // get pointer to element at index
*p = 10;                    // modify in place

V_SET(v, 99, 2);            // replace element at index 2
```

---

## Iterating

```c
// Forward
vector_iterator it = V_BEGIN(v);
while (V_HAS_NEXT(it)) {
    int val = V_NEXT_ELEM(it, int);
}

// Reverse
vector_iterator it = V_RBEGIN(v);
while (V_HAS_PREV(it)) {
    int val = V_PREV_ELEM(it, int);
}
```

---

## Useful Operations

```c
V_PUSH_ALL(v, arr, count);        // push a C array in bulk
V_INSERT(v, val, index);          // insert at index
V_REMOVE(v, index);               // remove at index

V_SORT(v, vector_cmp_int);        // sort (built-in comparators: cmp_int, cmp_double, cmp_str)
V_REVERSE(v);                     // reverse in place
V_APPLY_TO_ALL(v, my_fn);         // call a function on every element
vector* f = V_FILTER(v, my_pred); // returns a new filtered vector
V_FIND(v, val, cmp);              // linear search, returns index or -1
V_SEARCH(v, val, cmp);            // binary search (sorted arrays only)
```

---

## Memory Management

```c
V_RESERVE(v, 20);      // pre-allocate capacity without changing size
V_RESIZE(v, 10);       // set size (zero-initialises new slots)
V_SHRINK(v);           // release unused capacity

V_CLEAR(v);            // destroy all elements, reset size (keeps vector alive)
V_DELETE(v);           // destroy all elements and free the vector itself
```

Always call `V_DELETE` when done. If you used `V_CREATE`, never call `free()` on the vector manually.

---

## Custom Types (Deep Copy)

If your struct contains heap-allocated fields, provide a `copy` and `destroy` function:

```c
void my_copy(void* dst, void* src) { /* deep copy fields */ }
void my_destroy(void* elem)        { /* free internal fields only, not the struct itself */ }

vector* v = V_CREATE_CUSTOM(MyType, my_copy, my_destroy, NULL, NULL, NULL);
```

---

## Size and Capacity

```c
V_SIZE(v)       // number of elements currently stored
V_CAPACITY(v)   // allocated slots
V_IS_EMPTY(v)   // returns 1 if empty
V_IS_SORTED(v, cmp, 0)  // 0 = ascending check, 1 = descending
```
