# Vector Library: How to use

### INSTRUCTIONS TO USE THIS LIBRARY IN YOUR PROJECT
Include these in your file:
```c
#include "vectors.h"
#include "vectors_macros.h"
```

### COMPILING AND RUNNING
Make sure vectors.c vectors.h and vectors_macros.h are in the same directory
**To compile:**
```
make build SRC=myfile.c OUT=myprogram
```

**To run:**
```
./myprogram
```

### 3 MAINS ALREADY EXIST TO TEST OUT FUNCTIONALITY
- **Main1** — `Custom create`, `Push`, `Pop`, `Destroy`
- **Main2** — `Push All`, `Get`, `Set`, `Find`, `Binary Search`, `Sort`, `Reverse`
- **Main3** — `Iterators`, `Reserve`, `Clear`, `Apply to all`, `Filter`
  
*How to run:*
```
make main1
./main1
```

## Create 
```c
// Stack allocation 
V_INIT(&v, mytype);
V_INIT_CUSTOM(&v, mytype, copy_fn, destroy_fn, alloc_fn, realloc_fn, free_fn);

// Heap allocation (use V_DELETE to free)
vector* v = V_CREATE(mytype);
vector* v = V_CREATE_CUSTOM(mytype, copy_fn, destroy_fn, alloc_fn, realloc_fn, free_fn);
```

Pass `NULL` for any custom function to use the default

## Custom Types
If your struct contains heap-allocated fields, provide copy and destroy:
```c
void my_copy(void* dst, void* src) { /* deep copy internal fields into dst */ }
void my_destroy(void* elem)        { /* free internal fields only, not the struct */ }
```

## Free 
```c
V_CLEAR(v);          // destroy all elements and reset size, vector stays alive
V_DELETE(v);         // destroy all elements and free the vector too
```

## Add and Remove
```c
V_PUSH(v, val);                  // add to end
V_PUSH_ALL(v, arr, array_size);  // append entire array
V_POP(v);                        // remove and free last element
V_INSERT(v, val, index);         // insert at position
V_REMOVE(v, index);              // remove at position
```

## Access elements
```c
int* p = V_GET(v, 0, int);       // pointer to element at index
V_SET(v, val, index);            // replace element at index and destroy old one
```

## Memory Control
```c
V_RESERVE(v, n);     // pre allocate space for n elements
V_RESIZE(v, n);      // set size to n, initialise new slots to zero or destroy slots greater than index
V_SHRINK(v);         // set capacity equal to size
V_SIZE(v)            // returns current number of elements
V_CAPACITY(v)        // returns number of allocated slots
```

## Iterators
```c
// Forward
vector_iterator it = V_BEGIN(v);
while (V_HAS_NEXT(it))
{
    int x = V_NEXT_ELEM(it, int);   // read and advance
            //OR
  //void* p = V_NEXT(it);           // raw pointer version
}

// Reverse
vector_iterator it = V_RBEGIN(v);
while (V_HAS_PREV(it))
{
    int x = V_PREV_ELEM(it, int);   // read and go back
            //OR
    void* p = V_PREV(it);
}

V_JUMP(it, n);                      // jump forward n steps (negative = jump back)
```

## Search
```c
V_FIND(v, val, cmp);      // linear search — returns index or -1
V_SEARCH(v, val, cmp);    // binary search (array must be sorted) — returns index or -1
```

## Sort & Order
```c
V_SORT(v, cmp);                // mergesort in place
V_REVERSE(v);                  // reverse in place
V_IS_SORTED(v, cmp, reverse);  // reverse -> 0 = ascending, 1 = descending; returns 1 on success
```

## Functional Helpers
```c
V_APPLY_TO_ALL(v, fn);         // call fn(elem) on every element
vector* f = V_FILTER(v, pred); // new vector with elements where pred(elem) != 0

                /* remember to V_DELETE the result */
```

## Checks
```c
V_IS_EMPTY(v)                  // returns 1 if size is 0
V_IS_SORTED(v, cmp, reverse)   // returns 1 if sorted
```

## Built-in comparators: 
`vector_cmp_int`
`vector_cmp_double`
`vector_cmp_str`


