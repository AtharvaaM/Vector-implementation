#include "vectors.h"
#include "vectors_macros.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct bigdata
{
    int id;
    double value;
    char* name;
    char** tags;
    size_t tagcount;
} data;


void data_copy(void* dst, void* src)
{
    data* data1 = (data*) src;
    data* datanew = (data*)dst;
    datanew->id = data1->id;
    datanew->value = data1->value;
    datanew->tagcount = data1->tagcount;

    datanew->name = strdup(data1->name);

    datanew->tags = (char**) malloc(sizeof(char*) * data1->tagcount);

    for (size_t i = 0; i<data1->tagcount; i++)
    {
        datanew->tags[i] = strdup(data1->tags[i]);
    }

    printf("Data copied\n");

}

void data_destroy(void* elem) //destroys internal mallocs only
{
    data* data1 = (data*)elem;
    for (size_t i = 0; i<data1->tagcount; i++)
    {
        free(data1->tags[i]);
    }
    free(data1->tags);

    free(data1->name);

    printf("Data destroyed\n");
}

void* data_alloc(size_t size)
{
    printf("%zu bytes allocated\n", size);
    return malloc(size);
}

void* data_realloc(void* dst, size_t size)
{
    printf("%zu bytes reallocated\n", size);
    return realloc(dst, size);
}

void data_free(void* elem) //always runs twice, once on the entire allocated block of memory and then on the vector struct(once is we use vector_init instead of create)
{
    printf("Element freed\n");
    free(elem);
}

void print_data_vector(vector* v) 
{
    printf("VECTOR CONTAINS %zu ELEMENTS\n", V_SIZE(v));
    for (size_t i = 0; i < V_SIZE(v); i++) {
        data* d = V_GET(v, i, data); 
        printf("%d : ID: %d | Value: %.2f | Name: %s\n", i, d->id, d->value, d->name ? d->name : "NULL");
        if (d->tagcount > 0 && d->tags) {
            printf("Tags: ");
            for (size_t j = 0; j < d->tagcount; j++) {
                printf("%s ", d->tags[j] ? d->tags[j] : "NULL");
            }
            printf("\n");
        } else {
            printf("Tags: NONE\n");
        }
    }
    printf("\n\n");
}

int main()
{
    vector* v = V_CREATE_CUSTOM(data, data_copy, data_destroy, data_alloc, data_realloc, data_free);

    data data1;
    data1.id = 1;
    data1.value = 67.67;
    data1.name = "atharva";
    char* tags1[] = {"abc", "pqr", "xyz", "peak"};
    data1.tags = tags1;
    data1.tagcount = 4;

    V_PUSH(v, data1);

    data data2;
    data2.id = 2;
    data2.value = 676.76;
    char* name2 = strdup("kushal");
    data2.name = name2;
    char** tags2 = (char**) malloc(sizeof(char*) * 5);
    tags2[0] = strdup("start");
    tags2[1] = strdup("qwe");
    tags2[2] = strdup("rty");
    tags2[3] = strdup("uio");
    tags2[4] = strdup("asd");
    data2.tags = tags2;
    data2.tagcount = 5;

    V_PUSH(v, data2); //deep copy
    free(data2.name);
    for(int i=0; i<5; i++) free(data2.tags[i]);
    free(data2.tags);

    data d3;
    d3.id = 30;
    d3.value = 0.0;
    d3.name = strdup("no_tags");
    d3.tags = NULL;
    d3.tagcount = 0;

    V_PUSH(v, d3);
    free(d3.name);
        
    data d4;
    d4.id = 40;
    d4.value = 123.456;
    d4.name = strdup("many_tags");
    d4.tagcount = 6;
    d4.tags = malloc(sizeof(char*) * d4.tagcount);
    for (int i = 0; i < d4.tagcount; i++) {
        char buf[20];
        sprintf(buf, "tag_%d", i);
        d4.tags[i] = strdup(buf);
    }

    V_PUSH(v, d4);
    free(d4.name);
    for(int i=0; i<6; i++) free(d4.tags[i]);
    free(d4.tags);


    printf("Vector initialisation ended\n");

    
    print_data_vector(v);


    V_POP(v);
    print_data_vector(v);

    

    V_DELETE(v);
}
