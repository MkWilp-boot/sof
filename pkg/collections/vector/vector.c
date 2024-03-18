#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "vector.h"

void vec_grow(vector_t *vec) {
    vec->cap *= 2;
    vec->ptr = realloc(vec->ptr, vec->elements_size * vec->cap);
}

void *vec_get(vector_t *vec, size_t index) {
    return (vec->ptr + (index * vec->elements_size));
}

void vec_add(vector_t *vec, void *element) {
    unsigned char *element_ptr = (unsigned char*)element;
    // Check if capacity needs to be increased
     if (vec->len == vec->cap) {
        vec_grow(vec);
    }
    // Calculate the offset for the new element
    size_t offset = vec->len * vec->elements_size;
    // Copy the element to the vector
    memcpy(vec->ptr + offset, element_ptr, vec->elements_size);
    // Increment the length of the vector
    vec->len++;
}

vector_t vec_new(size_t elements_size) {
    vector_t vec = {
        .cap = MINIMAL_CAP,
        .elements_size = elements_size,
        .len = 0,
        .ptr = calloc(MINIMAL_CAP, elements_size),
    };
    return vec;
}