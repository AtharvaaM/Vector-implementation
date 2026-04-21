#ifndef VECTOR_H
#define VECTOR_H

#include <stddef.h> 

//function pointers (defined in struct)
// (return type) (fn name) (args taken)
typedef void* (*vector_alloc)(size_t size);
typedef void* (*vector_realloc)(void *ptr, size_t size);
typedef void (*vector_free)(void *ptr);
typedef void (*vector_copy)(void *dst, void *src);
typedef void (*vector_destroy)(void *elem);

//structs

typedef struct vector
{
    void* data;
    size_t size;
    size_t capacity;
    size_t step;
    vector_alloc v_alloc;
    vector_realloc v_realloc;
    vector_free v_free;
    vector_destroy v_destroy;
    vector_copy v_copy;
} vector;

typedef struct vector_iterator
{
    void* current;
    void* begin;
    void* end;
    size_t step;
} vector_iterator;

//create and initialise

//stack allocation
void vector_default_init(vector* v, size_t step);
void vector_custom_init(vector* v, size_t step, vector_copy copy_fn, 
                        vector_destroy destroy_fn, vector_alloc alloc_fn, 
                        vector_realloc realloc_fn, vector_free free_fn);
//heap allocation
vector* vector_default_create(size_t step);
vector* vector_custom_create(size_t step, vector_copy copy_fn, 
                            vector_destroy destroy_fn, vector_alloc alloc_fn, 
                            vector_realloc realloc_fn, vector_free free_fn);

//memory control

void vector_reserve(vector* v, size_t new_cap);
void vector_resize(vector* v, size_t new_cap);
void vector_shrink_to_fit(vector* v);

//free memory

void vector_clear(vector* v);
void vector_delete(vector* v);

//iterators

vector_iterator vector_begin(vector* v);
vector_iterator vector_rbegin(vector* v);
int vector_itr_has_next(vector_iterator* it);
void* vector_itr_next(vector_iterator* it);
int vector_itr_has_prev(vector_iterator* it);
void* vector_itr_prev(vector_iterator* it);
void vector_itr_jump(vector_iterator* it, int jumpsize);

//comparators

int vector_cmp_int(void* a, void* b);
int vector_cmp_double(void* a, void* b);
int vector_cmp_str(void* a, void* b);

//push, pop

void vector_push(vector* v, void* elem);
void vector_pop(vector* v);
void vector_push_all(vector* v, void* elt_array, size_t size);

//get, set

void* vector_get(vector* v, size_t index);
void vector_set(vector* v, void* elem, size_t index);

//insert,remove

void vector_insert(vector* v, void* elem, size_t index);
void vector_remove(vector* v, size_t index);

//search

int vector_find(vector* v, void* target, int (*cmp)(void*, void*));
int vector_bin_search(vector* v, void* target, int (*cmp)(void*, void*));

//sort

void vector_sort(vector* v, int (*cmp)(void*, void*));

//useful minor functions

void vector_reverse(vector* v);
void vector_for_each(vector* v, void (*function)(void* elem));
vector* vector_filter(vector* v, int (*predicate)(void* elem));

//checks

int vector_is_empty(vector* v);
int vector_is_sorted(vector* v, int (*cmp)(void*, void*), int reverse);


#endif
