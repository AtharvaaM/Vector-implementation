#ifndef VECTORS_MACROS_H
#define VECTORS_MACROS_H

#include "vectors.h"


//====================NOTE====================//
/*
    A do{} while(0) loop has been used wherever literals can be passed
    ({}) has been used when we have a return type and a literal can be used
*/
//============================================//


//CREATE------------------------------------------------------------------------------------------------
#define V_CREATE(type) vector_default_create(sizeof(type))

#define V_CREATE_CUSTOM(type, copy_fn, destroy_fn, alloc_fn, realloc_fn, free_fn) \
    vector_custom_create(sizeof(type), (copy_fn), (destroy_fn), (alloc_fn), (realloc_fn), (free_fn))

#define V_INIT(v, type) vector_default_init(v, sizeof(type))

#define V_INIT_CUSTOM(v, type, copy_fn, destroy_fn, alloc_fn, realloc_fn, free_fn) \
    vector_custom_create(v, sizeof(type), (copy_fn), (destroy_fn), (alloc_fn), (realloc_fn), (free_fn))

//FREE---------------------------------------------------------------
#define V_DELETE(v) vector_delete(v)

#define V_CLEAR(v) vector_clear(v)

//MEMORY CONTROL-----------------------------------------------------
#define V_RESERVE(v, n) vector_reserve((v), (n)) //() ->fixes precedence bugs

#define V_RESIZE(v, n) vector_resize((v),(n))

#define V_SHRINK(v) vector_shrink_to_fit(v)

#define V_SIZE(v) (v->size)

#define V_CAPACITY(v) (v->capacity)

#define V_IS_EMPTY(v) vector_is_empty(v)


//ITERATORS----------------------------------------------------------
#define V_BEGIN(v) vector_begin(v)

#define V_RBEGIN(v) vector_rbegin(v)

#define V_HAS_NEXT(it) vector_itr_has_next(&it)

#define V_NEXT(it) vector_itr_next(&it)

#define V_NEXT_ELEM(it, type) *(type*) vector_itr_next(&it)

#define V_HAS_PREV(it) vector_itr_has_prev(&it)

#define V_PREV(it) vector_itr_prev(&it)

#define V_PREV_ELEM(it, type) *(type*) vector_itr_prev(&it)

#define V_JUMP(it, n) vector_itr_jump(&(it), (n))

//PUSH AND POP-----------------------------------------------------

/*we need to put the entire expression into a brace so that it doesnt 
split up when something like if(condn) V_PUSH() is called*/
//also we need it to not interfere with if-else architecture
//it also needs to handle literals so we first put it into a variable
//thus we use the standard do{} while(0) format
#define V_PUSH(v, val) \
    do { \
        __typeof__(val) temp = (val); \
        vector_push((v), (&temp)); \
    } while (0) \

#define V_PUSH_ALL(v, arr, arr_size) vector_push_all((v), (arr), (arr_size))

#define V_POP(v) vector_pop(v)

//GET AND SET------------------------------------------------------
#define V_GET(v, index, type) (type*)vector_get(v,index) //need to know type (returns pointer)

//same as push
#define V_SET(v, val, index) \
    do { \
        __typeof__(val) temp = (val); \
        vector_set((v), (&temp), (index)); \
    } while (0) \


//INSERT AND REMOVE
#define V_INSERT(v, val, index) \
    do { \
        __typeof__(val) temp = (val); \
        vector_insert((v), (&temp), (index)); \
    } while (0) \

#define V_REMOVE(v, index) vector_remove((v), (index))

//SEARCH-----------------------------------------------------

//we define our statements inside ({}) whenever the macro has a return type
#define V_FIND(v, val, cmp) \
    ({ \
        __typeof__(val) temp = (val); \
        vector_find((v), (&temp), (cmp)); \
    }) \

#define V_SEARCH(v, val, cmp) \
    ({ \
        __typeof__(val) temp = (val); \
        vector_bin_search((v), (&temp), (cmp)); \
    }) \

//SORT--------------------------------------------------
#define V_SORT(v, cmp) vector_sort(v, cmp)

#define V_IS_SORTED(v, cmp, rev) vector_is_sorted((v),(cmp), (rev))

//MINOR FUNCTIONS---------------------------------------
#define V_REVERSE(v) vector_reverse(v)

#define V_APPLY_TO_ALL(v, fn) vector_for_each((v),(fn))

#define V_FILTER(v, fn) vector_filter((v),(fn))

#endif

