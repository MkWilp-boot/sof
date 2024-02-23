#ifndef SOF_COLLECTION_VECTOR_H
#define SOF_COLLECTION_VECTOR_H

#include <stddef.h>
#include <stdint.h>

#include "../collection.h"

typedef struct {
    array_t;
} vector_t;

vector_t vec_new(size_t elements_size);

void vec_grow(vector_t *vec);

void vec_add(vector_t *vec, void *element);

void *vec_get(vector_t *vec, size_t index);

#endif // SOF_COLLECTION_VECTOR_H