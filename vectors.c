#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "vectors.h" //struct and typedef functions defined here



//=========================CORE FUNCTIONS======================================
void* byte_pointer(vector* v, size_t index)
{
    return (char*) v->data + index*v->step;
}

void copy_element(vector* v, void* dst, void* src)
{
    if (v->v_copy) v->v_copy(dst, src);
    else memcpy(dst, src, v->step);
}

void destroy_element(vector* v, void* elem)
{
    if (v->v_destroy) v->v_destroy(elem);
}

int ensure_capacity(vector* v, size_t additional_data) //how many more elements
{
    if (v->capacity == 0) v->capacity  = 4;
    size_t temp = v->capacity;
    while (v->capacity < v->size + additional_data) v->capacity*= 2;

    size_t bytes = v->step * v->capacity;
    void* new;
    if (v->data == NULL) new = v->v_alloc(bytes);
    else new = v->v_realloc(v->data, bytes);

    if (new == NULL) 
    {
        fprintf(stderr, "Reallocation error in ensure_capacity\n");
        v->capacity = temp;
        return 1;
    }
    v->data = new;
    return 0;
}



//=========================CREATE AND INITIALISE===============================
void vector_default_init(vector* v, size_t step)
{
    v->data = NULL;
    v->size = 0;
    v->capacity = 0;
    v->step = step;

    v->v_copy = NULL;
    v->v_destroy = NULL;
    v->v_alloc = malloc;
    v->v_realloc = realloc;
    v->v_free = free;
}
 
void vector_custom_init(vector* v, size_t step, 
                        vector_copy copy_fn,
                        vector_destroy destroy_fn,
                        vector_alloc alloc_fn,
                        vector_realloc realloc_fn,
                        vector_free free_fn)
{
    vector_default_init(v, step);
    if (copy_fn) v->v_copy  = copy_fn;
    if (destroy_fn) v->v_destroy  = destroy_fn;
    if (alloc_fn) v->v_alloc = alloc_fn;
    if (realloc_fn) v->v_realloc = realloc_fn;
    if (free_fn) v->v_free = free_fn;
}

vector* vector_default_create(size_t step) 
{
    vector* v = (vector*)malloc(sizeof(vector));

    vector_default_init(v,step);
    return v;
}

vector* vector_custom_create(size_t step, 
                            vector_copy copy_fn, 
                            vector_destroy destroy_fn,
                            vector_alloc alloc_fn, 
                            vector_realloc realloc_fn, 
                            vector_free free_fn) 
{
    vector_alloc constructor = malloc;
    if (alloc_fn) constructor = alloc_fn;
    vector* v = (vector*)constructor(sizeof(vector));

    vector_custom_init(v, step, copy_fn, destroy_fn, alloc_fn, realloc_fn, free_fn);
    return v;
}



//=========================MEMORY CONTROL======================================
void vector_reserve(vector* v, size_t new_cap)
{
    if (new_cap <= v->capacity) return;
    void* new = v->v_realloc(v->data, new_cap*v->step);

    if (new == NULL)
    {
        fprintf(stderr, "Reallocation error in vector_reserve\n");
        return;
    }
    v->data = new;
    v->capacity = new_cap;
}

void vector_resize(vector* v, size_t new_cap)
{
    if (new_cap > v->size)  //initialise and allocate
    {
        if (new_cap > v->capacity)  //allocate
        {
            vector_reserve(v, new_cap); 
            if (v->capacity < new_cap) //vector_reserve failed 
            {
                fprintf(stderr, "vector_reserve failed in vector_resize\n");
                return; 
            }
        }

        size_t new_elements = new_cap - v->size; //initialise
        void* gap_start = byte_pointer(v, v->size);
        memset(gap_start, 0, new_elements*v->step);
    } 
    else if (new_cap < v->size)  //chop off end
    {
        for (size_t i = new_cap; i < v->size; i++) 
        {
            destroy_element(v, byte_pointer(v, i));
        }
    }
    
    v->size = new_cap;
}

void vector_shrink_to_fit(vector* v)
{
    if (v->size== 0)
    {
        vector_clear(v);
    }
    
    else if (v->size == v->capacity) return;

    else
    {
        void* new_data =v->v_realloc(v->data, v->step*v->size);

        if (new_data != NULL)
        {
            v->data = new_data;
            v->capacity = v->size;
        }

    }
}



//=========================FREE MEMORY=========================================
void vector_delete(vector* v)
{
    for (size_t i = 0; i<v->size; i++)
    {
        destroy_element(v, byte_pointer(v,i));
    }

    vector_free custom_free = v->v_free;
    if (v->data != NULL) custom_free(v->data);
    custom_free(v);
}

void vector_clear(vector* v) //make its capacity to 4*element size(default)
{
    for (size_t i = 0; i<v->size; i++)
    {
        destroy_element(v, byte_pointer(v, i));
    }
    void* new_data = v->v_realloc(v->data, 4* v->step);

    if (new_data != NULL) 
    {
        v->data = new_data;
        v->capacity = 4;
    }
    
    v->size = 0;
}



//=========================ITERATOR FUNCTIONS==================================
vector_iterator vector_begin(vector* v)
{
    vector_iterator it;
    it.begin = v->data;
    it.end = (char*)v->data + v->step * v->size;
    it.step = v->step;
    it.current = v->data;
    return it;
}

vector_iterator vector_rbegin(vector* v)
{
    vector_iterator it;
    it.begin = v->data;
    it.end = (char*)v->data - v->step;  //we dont need actual end
    it.step = v->step;
    if (v->size != 0) it.current = (char*)v->data + v->step * (v->size-1);
    else it.current = v->data;
    return it;
}

//move forward
int vector_itr_has_next(vector_iterator* it)
{
    return it->current<it->end;
}

void* vector_itr_next(vector_iterator* it) //reads and returns curr element before moving to next
{
    void* curr = it->current;
    it->current = (char*)it->current+it->step;
    return curr;
}

//move backwards
int vector_itr_has_prev(vector_iterator* it)
{
    return it->current > it->end;
}

void* vector_itr_prev(vector_iterator* it) //reads and returns curr element before moving to prev
{
    void* curr = it->current;
    it->current = (char*)it->current-it->step;
    return curr;
}

void vector_itr_jump(vector_iterator* it, int jumpsize) //jumpsize can be negative to jump back
{
    it->current = (char*) it->current + jumpsize*it->step;
}



//=========================COMPARATORS=========================================
int vector_cmp_int(void* a, void* b) 
{
    int x = *(int*)a;
    int y = *(int*)b;
    return (x > y)-(y>x);
}

int vector_cmp_double(void* a, void* b) {
    double v1 = *(double*)a;
    double v2 = *(double*)b;
    return (v1 > v2) - (v1 < v2);
}

int vector_cmp_str(void* a, void* b) 
{
    return strcmp(*(char**)a, *(char**)b);
}

int vector_cmp_raw(void* a, void* b, size_t n) 
{
    return memcmp(a, b, n);
}



//=========================PUSH AND POP========================================
void vector_push(vector* v, void* elem)
{
    if (ensure_capacity(v,1) != 0) //error
    {
        fprintf(stderr, "ensure_capacity failed in vector_push\n");
        return;
    }
    copy_element(v,byte_pointer(v, v->size) ,elem);
    v->size++;
}

void vector_push_all(vector* v, void* elt_array, size_t size)
{
    if (ensure_capacity(v, size)==1)
    {
        fprintf(stderr, "ensure_capacity failed in vector_push_all\n");
        return;
    }
    size_t index = v->size;
    for (size_t i = 0; i<size; i++)
    {
        copy_element(v, byte_pointer(v, index), (char*)elt_array+i*v->step);
        index++;
        v->size++;
    }
}

void vector_pop(vector* v)
{
    if (v-> size == 0) return;
    destroy_element(v, byte_pointer(v, v->size - 1));
    v->size --;
}



//=========================GET AND SET=========================================
void* vector_get(vector* v, size_t index)
{
    if (index >= v->size) return NULL;
    else return byte_pointer(v, index);
}

void vector_set(vector* v, void* elem, size_t index)
{
    if (index >= v->size) return;
    destroy_element(v, byte_pointer(v, index));
    copy_element(v, byte_pointer(v, index), elem);
}



//=========================INSERT AND REMOVE===================================
void vector_insert(vector* v, void* elem, size_t index) 
{
    if (elem == NULL) return;
    if (index > v->size) return; 
    else if (index == v->size) 
    {
        vector_push(v, elem); //behaves as push
        return;
    }
    else
    {
        if (ensure_capacity(v,1) == 1)
        {
            fprintf(stderr, "ensure_capacity failed in vector_insert\n");
            return;
        }
        memmove(byte_pointer(v, index+1), byte_pointer(v, index), (v->size - index)*v->step);
        copy_element(v, byte_pointer(v, index), elem);
        v->size++;
    }
}

void vector_remove(vector* v, size_t index)
{
    if (index >= v->size) return;
    else if (index == v->size -1) vector_pop(v);
    else
    {
        destroy_element(v,byte_pointer(v,index));
        memmove(byte_pointer(v,index), byte_pointer(v, index+1), (v->size - index-1)*v->step);
        v->size --;
    }
}



//=========================SEARCH==============================================
//there are 4 default comparators 
int vector_find(vector* v, void* target, int (*cmp)(void*, void*)) 
{
    for (size_t i = 0; i < v->size; i++) {
        if (cmp(byte_pointer(v, i), target) == 0) {
            return (int)i;
        }
    }
    return -1;
}

int vector_bin_search(vector* v, void* target, int (*cmp)(void*, void*)) //requires array to be sorted in ascending order
{ 
    if (v->size == 0) return -1; //prevent underflow
    
    int start = 0;
    int end = v->size - 1;
    int mid;

    while(start <= end)
    {
        mid = (start+end)/2;
        int comparison = cmp(byte_pointer(v, mid), target);
        if (comparison == 0) return (int)mid;
        else if (comparison > 0) 
        {
            end = mid - 1;
            continue;
        }
        else
        {
            start = mid + 1;
            continue;
        }
    }
    return -1;
}


//=========================SORT================================================
void vector_merge(void* base, size_t lcount, size_t rcount, 
                  size_t step, int (*cmp)(void*, void*)) 
{
    size_t i= 0; // left subarray
    size_t j= 0; // right subarray
    size_t k= 0; // final

    size_t lbytes = lcount*step;
    size_t rbytes = rcount*step;

    void* L =malloc(lbytes);
    void* R =malloc(rbytes);

    memcpy(L, base, lbytes);
    memcpy(R, (char*)base +lbytes, rbytes);

    while (i < lcount && j < rcount) 
    {
        void* left_ptr= (char*)L + (i*step);
        void* right_ptr= (char*)R + (j*step);

        if (cmp(left_ptr, right_ptr) <= 0) 
        {
            memcpy((char*)base + (k * step), left_ptr, step);
            i++;
        } 
        else 
        {
            memcpy((char*)base + (k * step), right_ptr, step);
            j++;
        }
        k++;
    }
    //clean up remaining elements
    while (i < lcount) 
    {
        memcpy((char*)base + (k *step), (char*)L + (i *step), step);
        i++;
        k++;
    }

    while (j < rcount) 
    {
        memcpy((char*)base + (k*step), (char*)R + (j*step), step);
        j++; 
        k++;
    }

    free(L);
    free(R);
}

void vector_mergesort(void* start, size_t total_elements, size_t step, int (*cmp)(void*, void*))
{
    if (total_elements <= 1) return;

    size_t lsize = total_elements/2;
    size_t rsize = total_elements-lsize;

    vector_mergesort(start, lsize, step, cmp); //sort left half

    void* right_start = (char*)start + step*lsize; //sort right half
    vector_mergesort(right_start, rsize, step, cmp);

    vector_merge(start, lsize, rsize, step, cmp); //merge both
}

void vector_sort(vector* v, int (*cmp)(void*, void*)) //wrapper
{
    vector_mergesort(v->data, v->size, v->step, cmp);
}



//=========================USEFUL MINOR FUNCTIONS==============================
void vector_reverse(vector* v)
{
    if (v->size <= 1) return;

    void* temp = malloc(v->step);
    if (temp == NULL)
    {
        fprintf(stderr, "malloc failed in vector_reverse\n");
        return;
    }

    for (size_t i = 0; i<v->size/2; i++)
    {
        size_t j = v->size-i-1;
        memcpy(temp, byte_pointer(v, i), v->step);
        memcpy(byte_pointer(v, i), byte_pointer(v, j), v->step);
        memcpy(byte_pointer(v, j), temp, v->step);

    }
    free(temp);
}

void vector_for_each(vector* v, void (*function)(void* elem))
{
    for (size_t i = 0; i < v->size; i++) function(byte_pointer(v, i));
}   

vector* vector_filter(vector* v, int (*predicate)(void* elem))
{
    vector* final = (vector*) v->v_alloc(sizeof(vector));

    if (final == NULL)
    {
        fprintf(stderr, "alloction failed in vector_filter\n");
        return NULL;
    }

    vector_custom_init(final, v->step, v->v_copy, v->v_destroy,
                        v->v_alloc, v->v_realloc, v->v_free);

    for (size_t i = 0; i< v->size; i++)
    {
        void* elem = byte_pointer(v, i);
        if (predicate(elem))
        {
            vector_push(final, elem);
        }
    }
    return final;
}



//=========================CHECKS==============================================
int vector_is_empty(vector* v)
{
    return v->size == 0;
}

int vector_is_sorted(vector* v, int (*cmp)(void*, void*), int reverse)
{
    if (v->size == 1) return 1;
    if (reverse == 0)
    {
        for (size_t i = 0; i<v->size -1; i++)
        {
            if (cmp(byte_pointer(v, i), byte_pointer(v, i+1)) > 0) return 0;
        }
        return 1;
    }
    for (size_t i = 0; i<v->size -1; i++)
    {
        if (cmp(byte_pointer(v, i), byte_pointer(v, i+1)) < 0) return 0;
    }
    return 1;
}