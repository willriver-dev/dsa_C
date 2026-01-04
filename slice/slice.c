#include <stdio.h>
#include <stdlib.h> 


typedef struct {
    int *data;      
    size_t len;    
    size_t cap; 
} IntVector;



void vector_init(IntVector *vec) {
    vec->len = 0;
    vec->cap = 4; 
    
    vec->data = (int *)malloc(vec->cap * sizeof(int));

    
    if (vec->data == NULL) {
        printf("Panic: Out of memory!\n");
        exit(1);
    }
}



void vector_push_back(IntVector *vec, int value) {
    
    if (vec->len == vec->cap) {
        

        
        
        size_t new_capacity = vec->cap * 2;

        
        
        
        int *newData = realloc(vec->data, new_capacity * sizeof(int));
        printf("--> Resize: Tang capacity tu %zu len %zu\n", vec->cap, new_capacity);

        vec->cap = new_capacity;
        vec->data = newData;
    }

    
    
    vec->data[vec->len] = value;

    vec->len++;
}


int vector_get(IntVector *vec, size_t index) {
    if (index >= vec->len) {
        printf("Panic: Index out of bounds!\n");
        exit(1);
    }

    
    return vec->data[index];
}




void vector_free(IntVector *vec) {
    
    vec->len = 0;
    vec->cap = 0;

    free(vec->data);
}


int main() {
    IntVector v;
    vector_init(&v);

    printf("--- Start Pushing ---\n");
    
    for (int i = 0; i < 10; i++) {
        vector_push_back(&v, i * 10);
        printf("Pushed %d (Size: %zu, Cap: %zu)\n", i * 10, v.len, v.cap);
    }

    printf("\n--- Reading Data ---\n");
    printf("Item at index 5: %d\n", vector_get(&v, 5));

    printf("\n--- Cleaning up ---\n");
    vector_free(&v);
    printf("Vector freed.\n");

    return 0;
}
