#include "vectors.h"
#include "vectors_macros.h"
#include <stdio.h>

int main() 
{
    vector* v = V_CREATE(int);

    int initial[] = {50, 10, 40, 20, 30};
    V_PUSH_ALL(v, initial, 5);
    V_PUSH(v, 5); 

    printf("Original: ");
    for(int i=0; i<V_SIZE(v); i++) 
    {
        printf("%d ", *V_GET(v, i, int));
    }
    printf("\n");

    printf("GET index 2: %d\n", *V_GET(v, 2, int));
    V_SET(v, 99, 2);
    printf("After replacing index 2: ");
    for(int i=0; i<V_SIZE(v); i++) 
    {
        printf("%d ", *V_GET(v, i, int));
    }
    printf("\n");
    V_SET(v, 40, 2); 

    int number = 10;
    int ind = V_FIND(v, number, vector_cmp_int);
    printf("Search for %d: found at index %d\n", number,ind);

    V_SORT(v, vector_cmp_int);
    printf("Sorted: ");
    for(int i=0; i<V_SIZE(v); i++) 
    {
        printf("%d ", *V_GET(v, i, int));
    }
    printf("\n");

    int target = 20;
    int index = V_SEARCH(v, target, vector_cmp_int); 
    printf("Binary Search for %d: found at index %d\n", target, index);

    V_REVERSE(v);
    printf("Reversed: ");
    for(int i=0; i<V_SIZE(v); i++) 
    {
        printf("%d ", *V_GET(v, i, int));
    }
    printf("\n");

    V_POP(v);
    printf("After pop: size = %zu\n", V_SIZE(v));

    printf("IS_EMPTY = %d\n", V_IS_EMPTY(v));
    while (!V_IS_EMPTY(v)) V_POP(v);
    printf("After popping all: IS_EMPTY = %d\n", V_IS_EMPTY(v));

    V_DELETE(v);
    return 0;
}