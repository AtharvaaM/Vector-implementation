#include "vectors.h"
#include "vectors_macros.h"
#include <stdio.h>
#include <stdbool.h>

void double_val(void* elem) 
{ 
    *(int*)elem *= 2; 
}

int is_even(void* elem) 
{ 
    return (*(int*)elem % 2 == 0); 
}

int main() 
{
    vector* v = V_CREATE(int);
    int initial[5] = {1, 2, 3, 4, 5};
    V_PUSH_ALL(v, initial, 5);

    printf("Forward Iteration : ");
    vector_iterator it = V_BEGIN(v);
    while (V_HAS_NEXT(it)) 
    {
        printf("%d ", V_NEXT_ELEM(it, int));
    }
    printf("\n");
    
    V_RESERVE(v, 20);
    printf("Capacity Expanded to %zu\n", V_CAPACITY(v));
    
    V_CLEAR(v);
    printf("Vector cleared; size is now %zu\n", V_SIZE(v));

    int data[] = {10, 15, 20, 25};
    V_PUSH_ALL(v, data, 4);

    V_APPLY_TO_ALL(v, double_val); 
    printf("Doubled values: ");
    vector_iterator it2 = V_BEGIN(v);
    while (V_HAS_NEXT(it2)) 
    {
        printf("%d ", V_NEXT_ELEM(it2, int));
    }
    printf("\n");
    
    *V_GET(v, 1, int) += 5;
    *V_GET(v, 3, int) -= 5;
    vector* evens = V_FILTER(v, is_even); 
    
    printf("Filtered elements: ");
    it = V_BEGIN(evens);
    while (V_HAS_NEXT(it)) 
    {
        printf("%d ", V_NEXT_ELEM(it, int));
    }
    printf("\n");

    V_DELETE(v);
    V_DELETE(evens);

    return 0;
}